GTEST_DIR = $(HOME)/local/glog

USER_DIR = .

CPPFLAGS += -I$(GTEST_DIR)/include
LIBS += -L$(GTEST_DIR)/lib

CXXFLAGS += $(LIBS) -g -Wall -pthread -lglog

TESTS = sample1

all : $(TESTS)

clean :
	rm -f $(TESTS) *.o

sample1 : sample1.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
