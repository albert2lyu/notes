LOCAL_PATH=/usr/local
CFLAGS=
all:
	g++ splitBygetLine.cpp -o splitBygetLine -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS)
	g++ token.cpp -o token -I$(LOCAL_PATH)/include -L$(LOCAL_PATH)/lib $(CFLAGS)

clean:
	rm -rf *.o splitBygetLine token
