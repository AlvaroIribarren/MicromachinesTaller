#include "EndScene.h"
#include "../Common/Constants.h"
#include <iostream>

EndScene::EndScene(SdlWindow& window) : 
	window(window),
	backgroundEndTex("end_screen.png", window),
	backgroundEnd(backgroundEndTex),
	_done(false),
	fullscreen(true) {}

bool EndScene::done() {
	return _done;
}

void EndScene::update() {
	window.getWindowSize(&xScreen, &yScreen);
	backgroundEnd.setDims(xScreen, yScreen);
}

void EndScene::draw() {
	window.fill();
	backgroundEnd.drawAt(xScreen/2, yScreen/2);
	window.render();
}

int EndScene::handle() {
	while (SDL_PollEvent(&e) && !_done) {
		if (e.type == SDL_QUIT) {
			_done = true;

		} else if (e.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (insideQuitButton(x, y)) {
				_done = true;
			}

		} else if (e.type == SDL_KEYDOWN) {
			SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) e;
			if (keyEvent.keysym.sym == SDLK_F11) {
				if (fullscreen) {
					window.setFullscreen(false);
					fullscreen = false;
				} else {
					window.setFullscreen(true);
					fullscreen = true;
				}
			}
		}
	}
	return SCENE_END;
}

bool EndScene::insideQuitButton(int x, int y) {
	Area playBtn(0.75*xScreen, 0.75*yScreen, 0.25*xScreen, 0.2*yScreen);
	return playBtn.isInside(x, y);
}