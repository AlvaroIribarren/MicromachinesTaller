#ifndef MICROMACHINES_GAMETHREAD_H
#define MICROMACHINES_GAMETHREAD_H

#include <vector>
#include "Car/Car.h"
#include "World.h"
#include "../Network/ThClient.h"
#include <thread>

class GameThread {
private:
    World _world;
    std::shared_ptr<Configuration> _configuration;
    std::list<std::shared_ptr<ClientThread>> finishedPlayers;

    bool _gameToStart, _gameStarted, _gameEnded;
public:
    GameThread(size_t n_of_players, const std::shared_ptr<Configuration>& configuration);
    void run(std::atomic_bool& acceptSocketRunning,
             std::atomic_bool& roomRunning,
             SafeQueue<std::shared_ptr<Event>>& incomingEvents,
             std::unordered_map<int ,std::shared_ptr<ClientThread>>& clients);

    ~GameThread();
    json getSerializedMap();
    std::shared_ptr<Car> createCar(int id, json j);
    void step();

    void startGame();

    void addToFinishedPlayersAndRemoveFromClients(
            std::unordered_map<int, std::shared_ptr<ClientThread>> &clients,
            int clientToBeRemovedId);
};

#endif //MICROMACHINES_GAMETHREAD_H
