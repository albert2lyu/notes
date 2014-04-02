#include <boost/thread.hpp> 
#include <iostream> 

#include <stdlib.h>

void wait(int seconds) 
{ 
    boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

int max = 500;
int count = 0;
boost::mutex mutex;

void thread(int id, int start, int end) 
{ 
    //printf("id: %02d - start: %02d, end: %02d\n", id, start, end);
    for (int i = start; i < end; ++i) 
    { 
        //wait(1); 
        printf("id: %02d - start: %02d, end: %02d, i: %02d\n", id, start, end, i);
        boost::unique_lock<boost::mutex> lock(mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) 
{
    if (argc == 2) {
        max = atoi(argv[1]);
    }
    int coreNum = boost::thread::hardware_concurrency() - 1;
    coreNum = 22;
    int per = max / coreNum;
    printf("per: %d, coreNum: %d, max: %d\n", per, coreNum, max);
    boost::thread_group threads;
    for (int i=0; i < coreNum; i++) {
        if (i != coreNum -1) {
            threads.create_thread(boost::bind(thread, i, i * per, (i+1) * per));
        } else {
            threads.create_thread(boost::bind(thread, i, i * per, max));
        }
    }
    threads.join_all();
    printf("count: %d\n", count);
} 
