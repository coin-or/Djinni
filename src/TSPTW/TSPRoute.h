#ifndef TSP_ROUTE_H
#define TSP_ROUTE_H

#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <iterator>
#include <sys/types.h>

#if defined(__GNUC__) && (__GNUC__ >= 4)
#include <tr1/memory>
#define SHARED_PTR std::tr1::shared_ptr
#else
#include <boost/shared_ptr.hpp>
#define SHARED_PTR boost::shared_ptr
#endif

#include "TSPTWWorld.h"
#include "../Twister/Twister.h"

//! A representation of information needed for the Traveling Salesman Problem.
/*! While many different WorldTypes can be used with TSPRoute, it has been most 
    thoroughly tested with TSPTWWorld.  Attempting to use other world types may 
	shake loose some interesting bugs.  Or they might not and our code could be
	perfect.  We don't know.  Don't panic, and have fun.

@author Hansen, Thiede
@since 2.0 
*/
template <class WorldType>
class TSPRoute {
    public:
        /*! A constructor that uses an already initialized World object.
        @param w A WorldType object */
        TSPRoute(WorldType& w) : 
            _w(new WorldType(w)), _time(0.0), 
            _cost(0.0), _timeWait(0.0)
        {
            _solution.resize(_w->data().size(), 0);
            _arrivaltime.resize(_solution.size(), 0);
            _penaltysum.resize(_solution.size(), 0);
            _identifier = "TSPRoute";
        }
        
        /*! A constructor that initializes a new WorldType.
        
        @param worldParam A char* containing parameters used to initialize a new object of type WorldType */
        TSPRoute(const char* worldParam) : 
            _w(new WorldType(worldParam)), 
            _time(0.0), _cost(0.0), _timeWait(0.0)
        {
            _solution.resize(_w->data().size(), 0);
            _arrivaltime.resize(_solution.size(), 0);
            _penaltysum.resize(_solution.size(), 0);
            _identifier = "TSPRoute";
        }
        
        /*! Virtualized for the benefit of future subclassing. */
        virtual ~TSPRoute() {}
        
        /*! Sets the Feasible component of the solution 
        @param f The new feasible component */
        void setF(const double& f) { _f = f; }
        
        /*! Sets the Penalty component of the solution
        @param p The new penalty component */
        void setP(const double& p) { _p = p; }
        
        /*! Returns the Feasible component of the solution
        @return The feasible component of the current solution */
        double getF() const { return _f; }
        
        /*! Returns the Penalty component of the solution
        @return the Penalty component of the current solution */
        double getP() const { return _p; }
        
        /*! Generates a neighbor TSPRoute from this current TSPRoute.
        @param neighbor The TSPRoute object which will receive the value.*/
        void generateNeighbor(TSPRoute& neighbor)
        {
            u_int32_t firstswitch = 0;
            u_int32_t secondswitch;
            int holder;
            u_int32_t numCustomers = _solution.size();
            neighbor.setF(getF());
            neighbor.setP(getP());
            while (0 == firstswitch)
                firstswitch = static_cast<int>((numCustomers - 1) * 
                    Twister::generateDouble()) + 1;
            secondswitch = firstswitch;
            while ((secondswitch == firstswitch) || 
                (secondswitch == firstswitch - 1))
                secondswitch = static_cast<int>((numCustomers - 1) *
                    Twister::generateDouble())+1;
            holder = _solution[firstswitch];
            if (firstswitch < secondswitch)
            {
                std::copy(_solution.begin(), _solution.begin() + firstswitch,
                    neighbor._solution.begin());
                std::copy(_solution.begin() + firstswitch + 1, 
                    _solution.begin() + secondswitch + 1,
                    neighbor._solution.begin() + firstswitch);
                neighbor._solution[secondswitch] = holder;
                neighbor._firstarrival = 
                    static_cast<u_int32_t>(_arrivaltime[firstswitch-1]);
                neighbor._firstpenalty =
                    static_cast<u_int32_t>(_penaltysum[firstswitch-1]);
                std::copy(_arrivaltime.begin(),
                    _arrivaltime.begin() + firstswitch,
                    neighbor._arrivaltime.begin());
                std::copy(_penaltysum.begin(),
                    _penaltysum.begin() + firstswitch,
                    neighbor._penaltysum.begin());
            } 
            else 
            {
                std::copy(_solution.begin(),
                    _solution.begin() + secondswitch + 1,
                    neighbor._solution.begin());
                std::copy(_solution.begin() + secondswitch + 1,
                    _solution.begin() + firstswitch,
                    neighbor._solution.begin() + secondswitch + 2);
                neighbor._solution[secondswitch+1] = holder;
                neighbor._firstarrival =
                    static_cast<u_int32_t>(_arrivaltime[secondswitch]);
                neighbor._firstpenalty =
                    static_cast<u_int32_t>(_penaltysum[secondswitch]);
                std::copy(_arrivaltime.begin(),
                    _arrivaltime.begin() + secondswitch + 1,
                    neighbor._arrivaltime.begin());
                std::copy(_penaltysum.begin(),
                    _penaltysum.begin() + secondswitch + 1,
                    neighbor._penaltysum.begin());
            }
            if (std::max(firstswitch, secondswitch) != numCustomers)
                std::copy(_solution.begin() + 
                    std::max(firstswitch, secondswitch) + 1, _solution.end(),
                    neighbor._solution.begin() + 
                    std::max(firstswitch, secondswitch) + 1);
            neighbor._firstswitch = firstswitch;
            neighbor._secondswitch = secondswitch;
            neighbor.update();
        }
        
        /*! Update schedules, member data, etc, based on current state. */
        void update()
        {
            double cost = getF();
            int firstswitch = _firstswitch;
            int secondswitch = _secondswitch;
            int numCustomers = _solution.size();
            const std::vector<int>& tour = _solution;
            const Matrix<double, 2>& travTime = 
                _w->travelTimes();
            if (firstswitch <= secondswitch)
            {
                if (secondswitch != (numCustomers - 1))
                {
                    cost -= (travTime[tour[firstswitch - 1]][tour[secondswitch]]
                        + travTime[tour[secondswitch]][tour[firstswitch]]
                        + travTime[tour[secondswitch-1]][tour[secondswitch+1]]);
                    cost += (travTime[tour[firstswitch - 1]][tour[firstswitch]]
                        + travTime[tour[secondswitch-1]][tour[secondswitch]] 
                        + travTime[tour[secondswitch]][tour[secondswitch+1]]);
                }
                else
                {
                    cost -= (travTime[tour[firstswitch - 1]][tour[secondswitch]]
                        + travTime[tour[secondswitch]][tour[firstswitch]]
                        + travTime[tour[secondswitch - 1]][tour[0]]);
                    cost += (travTime[tour[firstswitch - 1]][tour[firstswitch]]
                        + travTime[tour[secondswitch-1]][tour[secondswitch]]
                        + travTime[tour[secondswitch]][tour[0]]);
                }
                timingUpdate();
            }
            else
            {
                if(firstswitch != (numCustomers-1))
                {
                    cost -= (travTime[tour[secondswitch]][tour[secondswitch+2]]
                        + travTime[tour[firstswitch]][tour[secondswitch+1]]
                        + travTime[tour[secondswitch+1]][tour[firstswitch +1]]);
                    cost += (travTime[tour[firstswitch]][tour[firstswitch+1]]
                        + travTime[tour[secondswitch]][tour[secondswitch+1]]
                        + travTime[tour[secondswitch+1]][tour[secondswitch+2]]);
                }
                else
                {
                    cost -= (travTime[tour[secondswitch]][tour[secondswitch+2]]
                        + travTime[tour[firstswitch]][tour[secondswitch+1]]
                        + travTime[tour[secondswitch + 1]][tour[0]]);
                    cost += (travTime[tour[firstswitch]][tour[0]]
                        + travTime[tour[secondswitch]][tour[secondswitch+1]]
                        + travTime[tour[secondswitch+1]][tour[secondswitch+2]]);
                }
                timingUpdate();
            }
            setF(cost);
            setP(_penaltysum[numCustomers-1]);
        }
        
        /*! Randomize this TSPRoute. */
        void randomize()
        {
            for (u_int32_t i = 0 ; i < _solution.size() ; i += 1)
                _solution[i] = static_cast<int>(i);
            std::vector<int>::iterator iter = _solution.begin();
            ++iter;
            std::random_shuffle(iter, _solution.end(), Twister::stl_rng);
        }
        
        /*! Copy constructor.
        @param route The route to copy from. */
        TSPRoute(const TSPRoute<WorldType>& route) : 
            _w(route._w), _solution(route._solution),
            _f(route._f), _p(route._p),
            _identifier(route._identifier),
            _arrivaltime(route._arrivaltime),
            _penaltysum(route._penaltysum),
            _time(route._time), _cost(route._cost),
            _timeWait(route._timeWait),
            _firstswitch(route._firstswitch),
            _secondswitch(route._secondswitch),
            _firstarrival(route._firstarrival),
            _firstpenalty(route._firstpenalty)
        {}
        
        /*! Dump our current path to an output stream.
        @param os The output stream to dump our path to
        @return The output stream after we've dumped in it */
        std::ostream& dump(std::ostream& os) const
        {
            std::ostream_iterator<int> oiter(os, " ");
            std::ostream_iterator<int> end();
            std::copy(_solution.begin(), _solution.end(), oiter);
            return os;
        }
        
        /*! Computes the feasible and penalty portions of this TSPRoute. */
        void compute()
        {
            double addEnergy = 0;
            double energy = 0;
            double minutesMissed = 0;
            double routeTime = 0;
            double waitTime = 0;
            
            const Matrix<double, 2>& travTime = _w->travelTimes();
            const std::vector<double>& lowdeadlines = _w->lowDeadlines();
            const std::vector<double>& deadlines = _w->deadlines();
            
            _penaltysum[0] = 0;
            _arrivaltime[0] = 0;
            
            for (u_int32_t i = 0 ; i < _solution.size() - 1 ; ++i) 
            {
                energy += travTime[_solution[i]][_solution[i + 1]];
                routeTime += travTime[_solution[i]][_solution[i + 1]];
                _arrivaltime[i+1] = energy;
                if (energy < lowdeadlines[_solution[i+1]]) 
                {
                    addEnergy = lowdeadlines[_solution[i+1]] - energy;
                    waitTime += addEnergy;
                    energy += addEnergy;
                }
                if (energy > deadlines[_solution[i+1]])
                    minutesMissed += energy - deadlines[_solution[i+1]];
                _penaltysum[i+1] = minutesMissed;
            }
            energy += travTime[_solution[_solution.size() - 1]][_solution[0]];
routeTime += travTime[_solution[_solution.size() - 1]][_solution[0]];
            setF(routeTime);
            _time = routeTime;
            setP(minutesMissed);
            _timeWait = waitTime;
        }
        
    protected:
    
        /*! Update the travel schedule. */
        void timingUpdate()
        {
            int start;
            int numCustomers = _solution.size();
            const Matrix<double, 2>& travTime = 
                _w->travelTimes();
            const std::vector<double> & lowdeadlines = _w->lowDeadlines();
            const std::vector<double> & deadlines = _w->deadlines();
            const std::vector<int>& tour = _solution;
            
            if (_firstswitch < _secondswitch)
                start = _firstswitch;
            else
                start = _secondswitch + 1;
            
            _arrivaltime[start - 1] = _firstarrival;
            _penaltysum[start - 1] = _firstpenalty;
            for(int i = start; i <= numCustomers - 1; i++)
            {
                if (_arrivaltime[i-1] >= lowdeadlines[tour[i-1]])
                    _arrivaltime[i] = _arrivaltime[i-1] + 
                        travTime[tour[i-1]][tour[i]];
                else
                    _arrivaltime[i] = lowdeadlines[tour[i-1]] + 
                        travTime[tour[i-1]][tour[i]];
                if (_arrivaltime[i] > deadlines[tour[i]])
                    _penaltysum[i] = _penaltysum[i-1] + 
                        (_arrivaltime[i] - deadlines[tour[i]]);
                else
                    _penaltysum[i] = _penaltysum[i-1];
            }
        }

        SHARED_PTR<WorldType> _w;
        std::vector<int> _solution;
        double _f, _p;
        std::string _identifier;
        std::vector<double> _arrivaltime;
        std::vector<double> _penaltysum;
        double _time, _cost, _timeWait;
        u_int32_t _firstswitch, _secondswitch, _firstarrival, _firstpenalty;
};

/*! An operator<< overloaded for TSPRoute.

    @param os An output stream to write to
    @param sol A TSPRoute to write
    @return An output stream after we've written to it
*/
template <class WorldType>
std::ostream& operator<<(std::ostream& os, const TSPRoute<WorldType>& sol)
{
    return sol.dump(os);
}

/*! A more human-readable version of a fully qualified typename. */
typedef TSPRoute<TSPTWWorld> TravelingSalesman;

#endif

