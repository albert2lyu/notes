#include <boost/thread.hpp> 
#include <iostream> 

#include <stdlib.h>

void wait(int seconds) 
{ 
    boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

int count = 0;
boost::mutex mutex;

void thread(int id) 
{ 
    for (int i = 0; i < 500; ++i) 
    { 
        //wait(1); 
        //printf("id: %d - %d\n", id, i);
        boost::unique_lock<boost::mutex> lock(mutex);
        ++count;
    }
}

int main() 
{ 
    int coreNum = boost::thread::hardware_concurrency();
    //std::cout << "CPU Core Num: " << coreNum << std::endl;
    boost::thread_group threads;
    for (int i=0; i < coreNum * 12; i++) {
        threads.create_thread(boost::bind(thread, i));
    }
    threads.join_all();
    printf("count: %d\n", count);
} 
