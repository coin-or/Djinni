%module ca_tsp
%{
#include "Annealer.h"
#include "TSPRoute.h"
#include "Penalties.h"
%}

%include "Annealer.h"
%include "TSPRoute.h"
%include "Penalties.h"
%include "std_string.i"

%template(CA_TSP) Annealer<Compressed, TravelingSalesman>;



