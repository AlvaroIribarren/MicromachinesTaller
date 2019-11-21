#include "ReceiverThread.h"
#include "../Common/Constants.h"
#include <iostream>

ReceiverThread::ReceiverThread(SafeQueue<std::shared_ptr<SnapshotEvent>>& gameRecvQueue,
							   SafeQueue<std::shared_ptr<LobbySnapshot>>& lobbyRecvQueue,
							   SafeQueue<std::shared_ptr<EndSnapshot>>& endRecvQueue,
							   Protocol& protocol, Scene& currentScene) :
	gameRecvQueue(gameRecvQueue),
	lobbyRecvQueue(lobbyRecvQueue),
	endRecvQueue(endRecvQueue),
	protocol(protocol),
	_done(false),
	_isGameMode(false),
	_currentScene(currentScene) {}

void ReceiverThread::run() {

	//TODO: while (!done)
	//			switch
	//				while (curr scene)
	// para poder hacer lo de volver a escenas anteriores

	try {

		while(_currentScene == SCENE_LOBBY || _currentScene == SCENE_MENU) {
			std::shared_ptr<LobbySnapshot> lobbySnap(new LobbySnapshot(protocol));
			lobbyRecvQueue.push(lobbySnap);

			int id = lobbySnap->getMyId();
			if (lobbySnap->gameStarted(id)) {
				_currentScene = SCENE_GAME;
			}
		}
		while(_currentScene == SCENE_GAME) {
			std::shared_ptr<SnapshotEvent> gameSnap(new SnapshotEvent(protocol));
			gameRecvQueue.push(gameSnap);
		}
		while(_currentScene == SCENE_END) {
		    std::shared_ptr<EndSnapshot> endSnap(new EndSnapshot(protocol));
		    endRecvQueue.push(endSnap);
		}

	} catch (std::exception &e) {
		std::cerr << "Error from Receiver Thread" << e.what() << std::endl;
	} catch (...){
        std::cerr << "Unknown error from Receiver Thread" << std::endl;
	}
	_done = true;
}
bool ReceiverThread::finished() const {
	return _done;
}