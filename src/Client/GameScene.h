#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "SdlWindow.h"
#include "SdlTexture.h"
#include "View/BackgroundView.h"
#include "InputHandler.h"
#include "TextureCreator.h"
#include "PlayerDisplay.h"
#include "GameObjects.h"
#include "BotHandler.h"
#include "BaseScene.h"
#include "Audio.h"
#include <map>
#include <string>
#include "../Common/Converter.h"
#include "../Common/Event/SnapshotEvent.h"
#include "../Common/Event/Event.h"
#include "../Common/SafeQueue.h"
#include "PlayerDescriptor.h"
#include "ScreenRecorder.h"


class GameScene : public BaseScene {
private:
	SdlWindow& window;
	Audio audio;
	ScreenRecorder recorder;
	bool isDone;

	SafeQueue<std::shared_ptr<SnapshotEvent>>& recvQueue;
	SafeQueue<std::shared_ptr<Event>>& sendQueue;
	PlayerDescriptor& player;

	SdlTexture backgroundTex;
	BackgroundView background;
	PlayerDisplay display;

	InputHandler handler;
	TextureCreator creator;

	GameObjects gameObjects;
	BotHandler bot;

	Converter conv;
	int xScreen, yScreen;
	Scene nextScene;
	
	bool isGameOver;
	bool isMapReady;
	bool carExploded;

public:
	GameScene(SdlWindow& window, SafeQueue<std::shared_ptr<SnapshotEvent>>& recvQueue,
			  SafeQueue<std::shared_ptr<Event>>& sendQueue, PlayerDescriptor& player);
	bool done() override;
	void update() override;
	void draw() override;
	Scene handle() override;
	~GameScene() override = default;
private:
	void drawBackground();

	void updateCars(const CarStructList& cars);
	void updateGameEvents(const GameEventsList& gameEvents);
    void showMudSplat(const GameEventStruct& gameEvent);
	void addObject(const GameEventStruct& gameEvent);
	void removeObject(const GameEventStruct& gameEvent);
	void gameOver(const GameEventStruct& gameEvent);
	void lapCompleted(const GameEventStruct& gameEvent);
	void carExplosion(const GameEventStruct& gameEvent);


};

#endif // GAME_SCENE_H
