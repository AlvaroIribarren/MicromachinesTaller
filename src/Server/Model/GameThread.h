#ifndef MICROMACHINES_GAMETHREAD_H
#define MICROMACHINES_GAMETHREAD_H

#include <vector>
#include "Car/Car.h"
#include "World.h"
#include "Car/Tire.h"
#include "../../Common/Thread.h"

class GameThread {
private:
    World _world;
    std::vector<Car*> _cars;
    Tire* _tire;

public:
    GameThread(size_t n_of_players);
    void update(Input movInput, Input turnInput);

    void update(int control);
    std::vector<Car*> getCars();
    Tire* getTire();
    ~GameThread();
};

#endif //MICROMACHINES_GAMETHREAD_H