GTEST_DIR = $(HOME)/local/gtest

USER_DIR = .

CPPFLAGS += -I$(GTEST_DIR)/include
LIBS += -L$(GTEST_DIR)/lib

CXXFLAGS += $(LIBS) -g -Wall -pthread -lgtest -lgtest_main

TESTS = sample1_unittest

all : $(TESTS)

clean :
	rm -f $(TESTS) *.o

sample1.o : $(USER_DIR)/sample1.cc $(USER_DIR)/sample1.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1.cc

sample1_unittest.o : $(USER_DIR)/sample1_unittest.cc \
                     $(USER_DIR)/sample1.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1_unittest.cc

sample1_unittest : sample1.o sample1_unittest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
