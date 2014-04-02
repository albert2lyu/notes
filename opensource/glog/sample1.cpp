#include <glog/logging.h>

int main(int argc, char* argv[]) {
    //Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // ...
    LOG(INFO) << "Found cookies";
    VLOG(0) << "VLOG(0) Found cookies";
    VLOG(1) << "VLOG(1) Found cookies";
    DLOG(INFO) << "DLOG(INFO) Found cookies";
    LOG(ERROR) << "Found cookies";
}
