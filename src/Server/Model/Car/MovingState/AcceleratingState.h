#ifndef MICROMACHINES_ACCELERATINGSTATE_H
#define MICROMACHINES_ACCELERATINGSTATE_H

#include "CarMovingState.h"

class AcceleratingState : public CarMovingState{
public:
    std::shared_ptr<CarMovingState> handleInput(Car& car, const InputEnum& input);
    void update(Car& car);
};

#endif //MICROMACHINES_ACCELERATINGSTATE_H