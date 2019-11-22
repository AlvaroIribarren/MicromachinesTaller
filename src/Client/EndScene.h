#ifndef END_SCENE_H
#define END_SCENE_H

#include "BaseScene.h"
#include "SdlWindow.h"
#include "SdlTexture.h"
#include "View/BackgroundView.h"
#include "../Common/Event/EndSnapshot.h"
#include "TextureCreator.h"
#include "../Common/SafeQueue.h"
#include <map>

class EndScene : public BaseScene {
private:
	SdlWindow& window;
	SdlTexture backgroundEndTex;
	BackgroundView backgroundEnd;

	SafeQueue<std::shared_ptr<EndSnapshot>>& endRecvQueue;

	TextureCreator creator;
	std::map<ObjectType, ObjectViewPtr> carViews;

	std::vector<int> arrivedPlayers;

	SDL_Event e;
	bool _done;
	bool fullscreen;
	Scene nextScene;
	int xScreen, yScreen;
public:
	EndScene(SdlWindow& window, SafeQueue<std::shared_ptr<EndSnapshot>>& endRecvQueue);
	virtual bool done() override;
	virtual void update() override;
	virtual void draw() override;
	virtual Scene handle() override;
	virtual ~EndScene() {}
private:
	bool insideQuitButton(int x, int y);
	bool insideMenuButton(int x, int y);
	void drawCars();
};

#endif // END_SCENE_H
