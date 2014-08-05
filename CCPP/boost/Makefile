LOCAL_PATH=/Users/Hong/local/boost
CFLAGS=-lboost_thread -lboost_system
all:
	g++ example.cpp -o example -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS)
	g++ vector.cpp -o vector -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS)
	#g++ thread1.cpp -o thread1 -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS)
	#g++ thread2_interrupter.cpp -o thread2 -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS) 
	#g++ threadgroup.cpp -o threadgroup -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS) 

clean:
	rm -rf *.o example vector thread1 thread2 threadpool
