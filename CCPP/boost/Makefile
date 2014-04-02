LOCAL_PATH=/usr/local
CFLAGS=-lboost_thread -lboost_system
all:
	g++ thread1.cpp -o thread1 -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS) 
	g++ thread2_interrupter.cpp -o thread2 -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS) 
	g++ threadgroup.cpp -o threadgroup -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS) 

clean:
	rm -rf *.o thread1 thread2 threadpool
