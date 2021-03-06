#include "GameThread.h"
#include "../../Common/SocketError.h"
#include <iostream>
#include "../Network/ThClient.h"

using namespace std::chrono;

GameThread::GameThread(size_t n_of_players, 
                    const std::shared_ptr<Configuration>& configuration,
                    std::atomic_bool& acceptSocketRunning,
                    std::atomic_bool& roomRunning,
                    std::atomic_bool& gameStarted,
                    SafeQueue<std::shared_ptr<Event>>& incomingEvents,
                    std::unordered_map<int ,std::shared_ptr<ClientThread>>& clients,
                    RoomController& controller,
                    int roomId) :
                                             _configuration(configuration),
                                             _world(n_of_players, configuration),
                                             _worldDTO(),
                                             acceptSocketRunning(acceptSocketRunning),
                                             roomRunning(roomRunning),
                                             gameStarted(gameStarted),
                                             incomingEvents(incomingEvents),
                                             clients(clients),
                                             controller(controller),
                                             roomId(roomId){

}



void GameThread::run() {
    std::cout << "Running GameThread" << std::endl;
    ModsThread modsThread(LIBS_SRC + std::string("libs.txt"), &_worldDTO);
    std::shared_ptr<Event> event;
    std::shared_ptr<EndSnapshot> endSnapshot(new EndSnapshot);
    gameStarted = true;

    int i = 0;
    while (roomRunning && acceptSocketRunning) {
        try {
            std::shared_ptr<SnapshotEvent> snapshot(new SnapshotEvent);
            removeDeadClients();
            std::clock_t begin = clock();

            while (incomingEvents.get(event)) {
                handleEvent(event);
            }

            if (roomRunning) {
                checkLapCompletion(snapshot);
                step();
                createModifiers(modsThread, i);
				addPlayerModificationToSnapshot(snapshot, endSnapshot);
                eraseFinishedPlayersFromClients();

                for (auto &actualClient : clients) {
                    actualClient.second->sendSnapshot(snapshot);
                }
                for (auto &actualFinishedPlayer: finishedPlayers) {
                    actualFinishedPlayer.second->sendEndEvent(endSnapshot);
                }
                i++;

                std::clock_t end = clock();
                double execTime = double(end - begin) / (CLOCKS_PER_SEC / 1000);
                double frames = _configuration->getFPS();
                if (execTime < frames) {
                    int to_sleep = (frames - execTime);
                    std::this_thread::sleep_for(
                            std::chrono::milliseconds(to_sleep));
                }
            }
        } catch (SocketError &se) {
            roomRunning = false;
            std::cout << "SocketErrorFromGameTrhead" << se.what() <<std::endl;
        } catch (std::exception &e){
            roomRunning = false;
            std::cout << "Excepcion desde game thread: " << e.what() <<std::endl;
        } catch (...) {
            roomRunning = false;
            std::cerr << "Game Thread: UnknownException.\n";
        }
    }

    std::cout << "GameThread ready to be joined" << std::endl;
}

void GameThread::step(){
    _world.step(_configuration->getVelocityIterations(), _configuration->getPositionIterations());
    _world.toDTO(&_worldDTO);
}

void GameThread::applyPluginChanges() {
    _world.dtoToModel(&_worldDTO);
}


json GameThread::getSerializedMap() {
    return _world.getSerializedMap();
}

std::shared_ptr<Car> GameThread::createCar(int id, json j){
    return _world.createCar(id, j);
}

void GameThread::addToFinishedPlayers(
        std::unordered_map<int, std::shared_ptr<ClientThread>> &clients,
        int clientToBeRemovedId) {
    finishedPlayers.insert({clientToBeRemovedId, clients.at(clientToBeRemovedId)});
}


void GameThread::handleEvent(const std::shared_ptr<Event>& event) {
    Type type = (Type) event->j["type"].get<int>();
    int clientId = event->j["client_id"].get<int>();

    switch (type){
        case COMMAND: {
            if (clients.count(clientId)){
                std::shared_ptr<ClientThread> client = clients.at(clientId);
                client->handleEvent(event);
            }
            break;
        }
        case MENU: {
            std::shared_ptr<ClientThread> finishedPlayer = finishedPlayers.at(clientId);
            controller.addExistentClient(finishedPlayer);
            finishedPlayers.erase(clientId);
            checkIfRoomMustBeClosed();
            break;
        }
        default:
            throw std::runtime_error("Handle event error, tipo no conocido");
    }
}


void GameThread::removeDeadClients(){
    bool thereWasADeadOne = false;
    for (auto &actualClient: clients) {
        if (actualClient.second->isDead()) {
            actualClient.second = nullptr;
            thereWasADeadOne = true;
        }
    }

    if (thereWasADeadOne){
        std::vector<int> idsToEliminate;
        for (auto &actual : clients){
            if (actual.second == NULL){
                idsToEliminate.push_back(actual.first);
            }
        }

        for (auto &actualId: idsToEliminate){
            clients.erase(actualId);
        }

        checkIfRoomMustBeClosed();
    }
}

void GameThread::checkIfRoomMustBeClosed() {
    if (clients.empty() && finishedPlayers.empty()){
        finishedPlayers.clear();
        incomingEvents.clear();
        roomRunning = false;
        controller.eraseRoom(roomId);
    }
}

void GameThread::createModifiers(ModsThread& modsThread, int step) {
	if ((step % _configuration->getModifiersCreationFrequency()) == 0) {
		modsThread.run();
		applyPluginChanges();

		for (size_t j = 0; j < MAX_MODIFIERS; ++j) {
			ModifierDTO modifier = _worldDTO.modifiers[j];
			if (modifier.newModifier) {
				_worldDTO.modifiers[j].newModifier = false;
				for (auto &actualClient : clients) {
					actualClient.second->createModifier(modifier.type, modifier.id, modifier.x, modifier.y,
														modifier.angle);
				}
			}
		}
	}
}

void GameThread::addPlayerModificationToSnapshot(const std::shared_ptr<SnapshotEvent>& snapshot,
												 const std::shared_ptr<EndSnapshot>& endSnapshot) {
	for (auto &actualClient : clients) {
		actualClient.second->modifySnapshotFromClient(snapshot);
		if (actualClient.second->finishedPlaying()) {
			std::cout << "Player finished" << std::endl;
			int clientId = actualClient.first;
			actualClient.second->sendSnapshot(snapshot);
			addToFinishedPlayers(clients, clientId);
			endSnapshot->addPlayerFinished(actualClient.second->getIdFromRoom());
		}
	}
}

void GameThread::checkLapCompletion(const std::shared_ptr<SnapshotEvent> &snapshot) {
	for (auto &client: clients) {
		bool lapCompleted = client.second->update();
		if (lapCompleted) {
			int numberOfLapsFromClient = client.second->getNumberOfLaps();
			snapshot->setLapNumber(client.second->getIdFromRoom(), numberOfLapsFromClient);
		}
	}
}

void GameThread::eraseFinishedPlayersFromClients() {
	for (auto &actualFinishedPlayer: finishedPlayers) {
		if (clients.count(actualFinishedPlayer.second->getClientId()))
			clients.erase(actualFinishedPlayer.second->getClientId());
	}
}


GameThread::~GameThread() = default;

