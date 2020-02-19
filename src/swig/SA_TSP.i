%module sa_tsp
%{
#include "Annealer.h"
#include "TSPRoute.h"
#include "Penalties.h"
%}

%include "Annealer.h"
%include "TSPRoute.h"
%include "Penalties.h"
%include "std_string.i"

%template(SA_TSP) Annealer<Simulated, TravelingSalesman>;



