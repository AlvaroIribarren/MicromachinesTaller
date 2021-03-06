#include "SceneSelector.h"
#include "../Common/Constants.h"
#include "../Common/SocketError.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "LobbyScene.h"
#include "EndScene.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

SceneSelector::SceneSelector(int xScreen, int yScreen,
		const std::string& host, const std::string& port) : 
	window(xScreen, yScreen),
	currentScene(SCENE_MENU),
	protocol(host, port),
	lobbyRecvQueue(false),
	gameRecvQueue(false),
	endRecvQueue(false),
	sendQueue(true),
	receiver(gameRecvQueue, lobbyRecvQueue, endRecvQueue, protocol, currentScene),
	sender(sendQueue, protocol) {
		ScenePtr menu(new MenuScene(window, sendQueue));
		ScenePtr lobby(new LobbyScene(window, lobbyRecvQueue, sendQueue, player));
		ScenePtr game(new GameScene(window, gameRecvQueue, sendQueue, player));
		ScenePtr end(new EndScene(window, endRecvQueue, sendQueue, player));
		scenes.insert(std::make_pair(SCENE_MENU, menu));
		scenes.insert(std::make_pair(SCENE_LOBBY, lobby));
		scenes.insert(std::make_pair(SCENE_GAME, game));
		scenes.insert(std::make_pair(SCENE_END, end));
		
		receiver.start();
		sender.start();
	}

void SceneSelector::run() {
	try {
		while(!receiver.finished() && !sender.finished()) {
			std::clock_t begin = clock();

			ScenePtr scene;
		    scene = scenes.at(currentScene);
		    scene->update();
		    scene->draw();
		    currentScene = scene->handle();
		    if (scene->done()) {
		    	protocol.forceShutDown();
		    }

		    //Check exec time and sleep
		    std::clock_t end = clock();
		    double execTime = double(end - begin) / (CLOCKS_PER_SEC/1000);
		    if (execTime < 25) this->sleep(25 - execTime);
	    }
	} catch (SocketError& se) {
		std::cerr << "Socket error captured in SceneSelector" << std::endl;
	} catch (std::exception &e) {
		std::cerr << "Exception from scene selector: " << e.what() << std::endl;
	} catch (...){
        std::cerr << "Unknown error from scene selector" << std::endl;
    }
}

void SceneSelector::sleep(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

SceneSelector::~SceneSelector() {
	receiver.join();
	sendQueue.push(nullptr);
	sender.join();
}