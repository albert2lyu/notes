#include <boost/thread.hpp>
#include <iostream>
#include <exception>

void wait(int seconds)
{
    boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

void thread()
{
    try {
        for (int i=0; i<5; i++) {
            wait(1);
            std::cout << i << std::endl;
        }
    } catch (boost::thread_interrupted&) {
        std::cerr << "thread_interrupted " << std::endl;
    }
}

int main()
{
    boost::thread t(thread);
    wait(3);
    t.interrupt();
    t.join();
}
