#ifndef TSPTWWORLD_H
#define TSPTWWORLD_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sys/types.h>

//! A recursive templatized structure representing a matrix of arbitrary
//! dimensionality.

/*! Originally we had our own Matrix class to provide arbitrary dimensions,
    but implemented it via runtime checks and nonrecursive structures.  As
    it turns out this was precisely the wrong thing to do.  Switching to a
    recursive template resulted in immense performance improvements.

    @author Hansen
    @since 2.2.2
*/
template <typename T, const int N>
class Matrix
{
  public:
    typedef T value_type;
    Matrix() { }
    Matrix(std::vector<Matrix<T, N-1> > matrix) : _matrix(matrix) {}
#ifdef USE_BOUNDS_CHECKING
    const Matrix<T, N-1>& operator[](const u_int32_t n) const { return _matrix.at(n);}
    Matrix<T, N-1>& operator[](const u_int32_t n) { return _matrix.at(n); }
#else
    const Matrix<T, N-1>& operator[](const u_int32_t n) const { return _matrix[n]; }
    Matrix<T, N-1>& operator[](const u_int32_t n) { return _matrix[n]; }
#endif
    u_int32_t dimensions() const { return Matrix<T,N>::DIMENSIONS; }
    void reset()
    {
      for (u_int32_t i = 0 ; i < _matrix.size() ; i += 1) _matrix[i].reset();
    }
    u_int32_t size() const { return _matrix.size(); }
    void push_back(Matrix<T,N-1> matrix) { _matrix.push_back(matrix); }
    void resize(const u_int32_t n) { _matrix.resize(n, Matrix<T,N-1>()); }
  
 protected:
   static const u_int32_t DIMENSIONS = N;
   std::vector<Matrix<T, N-1> > _matrix;
};


//! A recursive templatized structure representing a one-dimensional matrix.

/*! Originally we had our own Matrix class to provide arbitrary dimensions,
    but implemented it via runtime checks and nonrecursive structures.  As
    it turns out this was precisely the wrong thing to do.  Switching to a
    recursive template resulted in immense performance improvements.

    @author Hansen
    @since 2.2.2
*/
template <typename T>
class Matrix<T, 1>
{
  public:
    typedef T value_type;
    Matrix() : _matrix() { }
    Matrix(std::vector<T> vec) : _matrix(vec) {}
#ifdef USE_BOUNDS_CHECKING
    const T& operator[](const u_int32_t n) const { return _matrix.at(n); }
    T& operator[](const u_int32_t n) { return _matrix.at(n); }
#else
    const T& operator[](const u_int32_t n) const { return _matrix[n]; }
    T& operator[](const u_int32_t n) { return _matrix[n]; }
#endif
    u_int32_t dimensions() const { return Matrix<T,1>::DIMENSIONS; }
    void reset() { _matrix.clear(); }
    u_int32_t size() const { return _matrix.size(); }
    void push_back(std::vector<T> vec) { _matrix.push_back(vec); }
    void resize(const u_int32_t n) { _matrix.resize(n, T()); }
  protected:
    std::vector<T> _matrix;
    static const u_int32_t DIMENSIONS = 1;
};  


//! A class representing an instance of the Traveling Salesman Problem with Time Windows.

/*! @author Hansen, Ohlmann, Thomas
    @since 1.0
*/
class TSPTWWorld
{
    public:
        TSPTWWorld(const std::string& filename);
        TSPTWWorld(const char* filename);
        virtual ~TSPTWWorld() {}
                
        const Matrix<double, 2>& travelTimes() const 
        { return _timeMatrix; }
        const std::vector<double>& lowDeadlines() const 
        { return _lowdeadlines; }
        const std::vector<double>& deadlines() const 
        { return _deadlines; } 
        
        //! Returns a const-reference to the Matrix used to store this world's data.
        const Matrix<double, 2>& data() const
        { return _matrix; }
        
        //! Returns a reference to the Matrix used to store this world's data.
        Matrix<double, 2>& data()
        { return _matrix; }
  
        //! Returns a const reference to the identifying string used for this World.
        const std::string& identifier() const
        { return _identifier; }

    protected:
        Matrix<double, 2> _timeMatrix;
        Matrix<double, 2> _matrix;
        std::vector<double> _lowdeadlines, _deadlines;
        std::string _identifier;
        
        virtual void computeTravelTimes()
        {
            u_int32_t numCustomers = _matrix.size();
            _timeMatrix.resize(numCustomers);
            for (u_int32_t i = 0; i <= numCustomers - 1; i++)
            {
                _timeMatrix[i].resize(numCustomers);
                for (u_int32_t j = 0; j <= numCustomers - 1; j++)
                {
                    _timeMatrix[i][j] =
                        ::sqrt((_matrix[i][0] - _matrix[j][0])*(_matrix[i][0] -
                            _matrix[j][0]) + (_matrix[i][1] - _matrix[j][1]) *
                            (_matrix[i][1] - _matrix[j][1]));
                    _timeMatrix[i][j] = ::floor(_timeMatrix[i][j]);
                }
            }
            for (u_int32_t i = 0 ; i <= numCustomers - 1 ; i++)
                for (u_int32_t j = 0 ; j <= numCustomers - 1 ; j++)
                    for (u_int32_t k = 0 ; k <= numCustomers - 1 ; k++)
                        if (_timeMatrix[i][j] >
                            (_timeMatrix[i][k] + _timeMatrix[k][j]))
                            _timeMatrix[i][j] =
                                _timeMatrix[i][k] + _timeMatrix[k][j];
            _lowdeadlines.resize(numCustomers);
            _deadlines.resize(numCustomers);
            for(u_int32_t i = 0; i <= numCustomers - 1; i++)
            {
#ifdef USE_BOUNDS_CHECKING
                _lowdeadlines.at(i) = _matrix[i][3];
                _deadlines.at(i) = _matrix[i][4];
#else
                _lowdeadlines[i] = _matrix[i][3];
                _deadlines[i] = _matrix[i][4];
#endif
            }
        }
};

std::ostream& operator<<(std::ostream& os, const TSPTWWorld& w);

#endif

