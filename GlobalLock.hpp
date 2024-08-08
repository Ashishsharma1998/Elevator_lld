#ifndef GLOBALLOCK_HPP
#define GLOBALLOCK_HPP

#include <mutex>

class GlobalLock {
public:
    static std::mutex mtx;
};

std::mutex GlobalLock::mtx;
#endif // GLOBALLOCK_HPP
