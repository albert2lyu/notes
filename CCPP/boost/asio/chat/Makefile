####### Compiler, tools and options

PROJECT_ROOT = `pwd` 
PLAT_3RD=$(HOME)/local/boost

CC	=	gcc
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -Wall -W -O2 -DNO_DEBUG
INCPATH	=	-I$(PLAT_3RD)/include -I$(PLAT_3RD)/protobuf/include
LINK	=	g++
LFLAGS	=	
LIBS	=	-L$(PLAT_3RD)/lib -lboost_program_options -lboost_thread -lboost_system -L$(PLAT_3RD)/protobuf/lib -lprotobuf
REMOVE	=	rm -f
TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files
SERVER	=chatServer	
CLIENT	=chatClient	

SERVER_SRC = ./chat_server.cpp

CLIENT_SRC = ./chat_client.cpp

SERVER_OBJECTS	= $(subst .cpp,.o, $(SERVER_SRC))
CLIENT_OBJECTS	= $(subst .cpp,.o, $(CLIENT_SRC))

####### Build rules


all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJECTS)
	$(REMOVE) $(SERVER) 
	$(LINK) $(LFLAGS) -o $(SERVER) $(SERVER_OBJECTS) $(LIBS)

$(CLIENT): $(CLIENT_OBJECTS)
	$(REMOVE) $(CLIENT) 
	$(LINK) $(LFLAGS) -o $(CLIENT) $(CLIENT_OBJECTS) $(LIBS)

clean:
	$(REMOVE) $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT)


####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Compile
#$(OBJECTS):

#%.cpp.o : %.cpp
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
#%.cc.o : %.cc
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
#
