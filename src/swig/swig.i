%module Djinni
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
%template(CA_TSP) Annealer<Compressed, TravelingSalesman>;
%template(SA_TSP) Annealer<Simulated, TravelingSalesman>;



