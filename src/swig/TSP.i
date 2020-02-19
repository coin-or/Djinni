%module tsp
%{
#include "Annealer.h"
#include "TSPRoute.h"
#include "Penalties.h"
%}

%include "Annealer.h"
%include "TSPRoute.h"
%include "Penalties.h"
%include "std_string.i"

%template(TSP) TSPRoute<TSPTWWorld>;



