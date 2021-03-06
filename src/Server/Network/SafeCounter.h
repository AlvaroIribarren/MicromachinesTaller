//
// Created by alvaro on 2/11/19.
//

#ifndef MICROMACHINES_SAFECOUNTER_H
#define MICROMACHINES_SAFECOUNTER_H


#include <mutex>

class SafeCounter {
private:
    int count;
    std::mutex m;
public:
    SafeCounter();
    int returnValueAndAddOne();
    ~SafeCounter();
};


#endif //MICROMACHINES_SAFECOUNTER_H
