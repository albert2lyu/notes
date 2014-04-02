#include <boost/thread.hpp> 
#include <iostream> 
#include <stdlib.h> 
#include <boost/algorithm/string.hpp>

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
    std::string str = "  Hello, World   ";
    printf("#%s#\n", str.c_str());
    boost::trim(str);
    printf("#%s#\n", str.c_str());
    std::cout << "CPU Core Num: " << boost::thread::hardware_concurrency() << std::endl;
    boost::thread t(thread); 
    t.join(); 
} 
