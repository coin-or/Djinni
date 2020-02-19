#!/usr/bin/python

from djinni.tsp import TSP
from djinni.ca_tsp import Compressed, CA_TSP

tsp = TSP("Dumas-1.set")
ca = Compressed(0.06, 0.0, 0.9999)
ca_tsp = CA_TSP(ca, tsp, 0.95, 0.94, 75, 100, 30000)
ca_tsp.solve()
print "Cost     = " + str(ca_tsp.cost())
print "Penalty  = " + str(ca_tsp.penalty())
print "Solution = " + ca_tsp.solution()
