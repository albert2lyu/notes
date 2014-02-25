GEOS=/Users/Hong/local/geos
CPPDB=/Users/Hong/local/cppdb
all:
	g++ -g -o example example.cpp -I$(GEOS)/include -L$(GEOS)/lib -I$(CPPDB)/include -L$(CPPDB)/lib -lgeos -lcppdb
	g++ -g -o geometry geometry.cpp -I$(GEOS)/include -L$(GEOS)/lib -I$(CPPDB)/include -L$(CPPDB)/lib -lgeos -lcppdb

clean:
	rm -rf *.o example *.dSYM
