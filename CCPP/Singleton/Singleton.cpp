/*
 * Singleton.cpp
 * From: https://github.com/augcampos/asterisk-cpp/blob/master/asterisk-cpp/src/structs/Singleton.cpp
 */

#include "Singleton.h"


NonCopyable::NonCopyable() {
}

NonCopyable::NonCopyable(const NonCopyable& c_dmn) {
}

const NonCopyable& NonCopyable::operator=(const NonCopyable& c_dmn) {
    return (*this);
}
