BUILT_SOURCES = swig_tsp.cpp swig_ca_tsp.cpp swig_sa_tsp.cpp __init__.py tsp.py ca_tsp.py sa_tsp.py
EXTRA_DIST = Doxyfile TSPTW/*.h Twister/*.h Twister/*.cc swig/*.i annealer/*.h examples/* doxygen/*.h swig/*.i
SWIG_SHARED_SOURCES = TSPTW/TSPTWWorld.cc Twister/Twister.cc
ADDL_CPPFLAGS=${BOOST_CPPFLAGS} -I..

pkgpython_PYTHON = tsp.py ca_tsp.py sa_tsp.py __init__.py
pkgpyexec_LTLIBRARIES = _tsp.la _ca_tsp.la _sa_tsp.la

_tsp_la_SOURCES=swig_tsp.cpp $(SWIG_SHARED_SOURCES)
_tsp_la_LDFLAGS=-module
_tsp_la_CPPFLAGS=$(SWIG_PYTHON_CPPFLAGS) -ITSPTW -ITwister -Iannealer $(ADDL_CPPFLAGS)

_ca_tsp_la_SOURCES=swig_ca_tsp.cpp $(SWIG_SHARED_SOURCES)
_ca_tsp_la_LDFLAGS=-module
_ca_tsp_la_CPPFLAGS=$(SWIG_PYTHON_CPPFLAGS) -ITSPTW -ITwister -Iannealer $(ADDL_CPPFLAGS)

_sa_tsp_la_SOURCES=swig_sa_tsp.cpp $(SWIG_SHARED_SOURCES)
_sa_tsp_la_LDFLAGS=-module
_sa_tsp_la_CPPFLAGS=$(SWIG_PYTHON_CPPFLAGS) -ITSPTW -ITwister -Iannealer $(ADDL_CPPFLAGS)

swig_tsp.cpp:
	$(SWIG) $(SWIG_PYTHON_OPT) -c++ -ITSPTW -ITwister -Iannealer -o swig_tsp.cpp swig/TSP.i

swig_ca_tsp.cpp:
	$(SWIG) $(SWIG_PYTHON_OPT) -c++ -ITSPTW -ITwister -Iannealer -o swig_ca_tsp.cpp swig/CA_TSP.i

swig_sa_tsp.cpp:
	$(SWIG) $(SWIG_PYTHON_OPT) -c++ -ITSPTW -ITwister -Iannealer -o swig_sa_tsp.cpp swig/SA_TSP.i

tsp.py: swig_tsp.cpp

ca_tsp.py: swig_ca_tsp.cpp

sa_tsp.py: swig_sa_tsp.cpp

__init__.py:
	touch __init__.py

clean-local:
	rm -f _Djinni* *.py swig_*.cpp

dist-hook:
	for file in $(BUILT_SOURCES) ; do \
		rm -f $(distdir)/$$file ; \
	done

docs: Doxyfile
	doxygen

