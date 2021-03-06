#ifndef MICROMACHINES_ROOMCONTROLLER_H
#define MICROMACHINES_ROOMCONTROLLER_H

#include <list>
#include <unordered_map>
#include <atomic>
#include "LobbyListener.h"
#include "../../Common/Thread.h"
#include "../../Common/SafeQueue.h"
#include "SafeCounter.h"
#include "../../Common/Protocol.h"
#include "../../Common/Event/CommandEvent.h"
#include "../../Common/Event/LobbySnapshot.h"
#include "Collector.h"
#include "../Model/Configuration.h"

class ClientThread;
class Room;
class Collector;

class RoomController {
private:
    std::unordered_map<int, std::shared_ptr<ClientThread>> clientsWithNoRoom;
    std::map<int, std::shared_ptr<Room>> rooms;
    std::list<std::shared_ptr<Collector>> collectors;
    SafeQueue<std::string> queue;
    SafeCounter clientCounter;
    std::atomic_bool& acceptSocketRunning;
    std::atomic_bool stopped;
    std::mutex m;
    LobbyListener listener;
    std::shared_ptr<Configuration> config;
    Collector collector;

    int getRoomIdOfClient(int clientId);

    void moveClientToNewRoom(int newRoomId, int clientId, int newPlayerIdFromRoom);

public:
    explicit RoomController(std::atomic_bool& running);

   int addRoom();

   void addClient(Protocol protocol);

   //Devuelve si se dio el comando play o no.
   bool handleInput(json j, std::shared_ptr<LobbySnapshot> snapshot);

    void sendToClientsWithoutRoom(std::shared_ptr<LobbySnapshot> snapshot);
    void sendToAllClientsWithRoom(std::shared_ptr<LobbySnapshot> snapshot);

    void addClientToRoom(int roomId, int clientId, int playerIdInRoom);

    int getIdFromRoom(int clientId);

    void addExistentClient(const std::shared_ptr<ClientThread>& client);

    int getAFreeRoomId();

    void eraseRoom(int roomId);

    ~RoomController();
};


#endif //MICROMACHINES_ROOMCONTROLLER_H
