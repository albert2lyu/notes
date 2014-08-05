CPPDB=/Users/Hong/local/cppdb
all:
	g++ -g -o select select.cpp -I$(CPPDB)/include -L$(CPPDB)/lib -lcppdb
	g++ -g -o connectionTerm connectionTerm.cpp -I$(CPPDB)/include -L$(CPPDB)/lib -lcppdb
	#g++ -g -o example example.cpp -I$(CPPDB)/include -L$(CPPDB)/lib -lcppdb
	#g++ -g -o cfgtable cfgtable.cpp -I$(CPPDB)/include -L$(CPPDB)/lib -lcppdb

clean:
	rm -rf *.o cfgtable example *.dSYM select
