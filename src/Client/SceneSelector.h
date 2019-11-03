#ifndef SCENE_SELECTOR_H
#define SCENE_SELECTOR_H

#include "SdlWindow.h"
#include "BaseScene.h"
#include "ReceiverThread.h"
#include "SenderThread.h"
#include "../Common/SafeQueue.h"
#include "../Common/Queue.h"
#include "../Common/Event/Event.h"
#include "../Common/Event/SnapshotEvent.h"
#include "../Common/Protocol.h"
#include <map>

class SceneSelector {
private:
	SdlWindow window;
	std::map<int, BaseScene*> scenes;

	Protocol protocol;

	Queue<SnapshotEvent*> recvQueue;
	SafeQueue<Event*> sendQueue;

	ReceiverThread receiver;
	SenderThread sender;
	
	int currentScene;


public:
	SceneSelector(int xScreen, int yScreen,
		const std::string& host, const std::string& port);
	void run();
	~SceneSelector();

private:
	void sleep(int milliseconds);
};

#endif // SCENE_SELECTOR_H