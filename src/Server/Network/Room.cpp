//
// Created by alvaro on 29/10/19.
//

#include <iostream>
#include <memory>
#include "Room.h"
#include "../../Common/Event/EventCreator.h"
#include "../../Common/Event/CommandEvent.h"
#include "../../Common/SocketError.h"

Room::Room(std::atomic_bool& acceptSocketRunning,
        int roomId, int amountOfPlayers,
        const std::shared_ptr<Configuration>& config,
        RoomController& controller): roomId(roomId),
                                              maxAmountOfPlayers(amountOfPlayers),
                                              acceptSocketRunning(acceptSocketRunning),
                                              roomRunning(true),
                                              incomingEvents(false),
                                              gameStarted(false),
                                              game(new GameThread(amountOfPlayers, 
                                                    config,
                                                acceptSocketRunning,
                                                roomRunning,
                                                gameStarted,
                                                incomingEvents,
                                                clients,
                                                controller,
                                                roomId)){}

void Room::run() {
    try{
        std::cout << "Room started" << std::endl;
        startGame();
        game->start();

    } catch (SocketError &se) {
        roomRunning = false;
        std::cout << "Room id: " << roomId << " (SE): " << se.what() << std::endl;
    } catch (std::exception &e){
        roomRunning = false;
        std::cout << "Room id: " << roomId << " (E): " << e.what() << std::endl;
    } catch (...) {
        roomRunning = false;
        std::cout << "Room id: " << roomId << " unknown exception" << std::endl;
    }
}

bool Room::hasClient(int clientId){
    return this->clients.count(clientId);
}

void Room::addClientAlreadyCreated(int clientId, std::shared_ptr<ClientThread> newClient) {
    if (clients.size() < maxAmountOfPlayers){
        this->clients.insert({clientId, newClient});
    } else {
        throw std::runtime_error("Se intento meter un cliente a una sala llena");
    }
}

void Room::addClient(int clientId, std::shared_ptr<ClientThread> newClient) {
    if (clients.size() < maxAmountOfPlayers){
        this->clients.insert({clientId, newClient});
    } else {
        throw std::runtime_error("Se intento meter un cliente a una sala llena");
    }
}



std::shared_ptr<Car> Room::createCar(int id, json j) {
    std::shared_ptr<Car> car(game->createCar(id, j));
    return car;
}

std::shared_ptr<ClientThread> Room::eraseClientAndReturn(int clientId) {
    std::shared_ptr<ClientThread> client = this->clients.at(clientId);
    this->clients.erase(clientId);
    return client;
}

void Room::sendSnapshotToClients(std::shared_ptr<LobbySnapshot>& snapshot){
    for (auto& client: clients){
        client.second->sendLobbySnapshot(snapshot);
    }
}

void Room::addPlayersToJson(json& j){
    int i = 0;
    for (auto& client: clients){
        int id = client.second->getIdFromRoom();
        j["cars"].at(i)["id_from_room"] = id;
        j["cars"].at(i)["color"] = getColourFromInRoomId(id);
        ++i;
    }
}

void Room::addCarToEachPlayer(const json& j){
    for (auto& client: clients){
        int id = client.second->getIdFromRoom();
        auto car = std::shared_ptr<Car>(createCar(id, j));
        client.second->assignCar(car);
    }
}

void Room::startGame() {
    json jFinal = game->getSerializedMap();
    addPlayersToJson(jFinal);

    addCarToEachPlayer(jFinal);

    for (auto& client: clients){
        client.second->assignRoomQueue(&incomingEvents);
        client.second->sendStart(jFinal);
    }
    this->gameStarted = true;
}

bool Room::isDead() {
    return !roomRunning;
}


int Room::getRoomIdFromClient(int clientId) {
    return clients.at(clientId)->getIdFromRoom();
}


int Room::getColourFromInRoomId(int id) {
    switch(id){
        case 0:
            return TYPE_CAR_RED;
        case 1:
            return TYPE_CAR_BLUE;
        case 2:
            return TYPE_CAR_YELLOW;
        case 3:
            return TYPE_CAR_GREEN;
        default:
            return -1;
    }
}


Room::~Room() {
    std::cout << "Entrando al destructor del room: " << roomId << std::endl;
    this->roomRunning = false;
    clients.clear();

    if (this->gameStarted) {
        game->join();
        std::cout << "Room: " << this->roomId << " joined."<< std::endl;
    } else {
        std::cout << "No se joineo el room: " << this->roomId << " porque el juego no comenzo."<< std::endl;
    }
}



