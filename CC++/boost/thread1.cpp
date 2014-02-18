#include <boost/thread.hpp> 
#include <iostream> 

void wait(int seconds) 
{ 
    boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

void thread() 
{ 
    for (int i = 0; i < 5; ++i) 
    { 
        wait(1); 
        std::cout << i << std::endl; 
    } 
} 

int main() 
{ 
    std::cout << "CPU Core Num: " << boost::thread::hardware_concurrency() << std::endl;
    boost::thread t(thread); 
    t.join(); 
} 
