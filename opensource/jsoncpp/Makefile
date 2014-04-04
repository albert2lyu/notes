GTEST_DIR = $(HOME)/local/jsoncpp

USER_DIR = .

CPPFLAGS += -I$(GTEST_DIR)/include
LIBS += -L$(GTEST_DIR)/lib

CXXFLAGS += $(LIBS) -g -Wall -pthread -ljson_linux-gcc-4.2.1_libmt 

TESTS = sample1 sample2

all : $(TESTS)

clean :
	rm -rf $(TESTS) *.o *.dSYM test_append.json

sample1 : sample1.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

sample2 : sample2.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
