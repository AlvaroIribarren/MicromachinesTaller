#ifndef __SDL_WINDOW_H__
#define __SDL_WINDOW_H__
#include "Area.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class SdlWindow {
public:
    int width;
    int height;
    /**
     * Ctor standalone
     */
    SdlWindow(int width, int height);
    ~SdlWindow();
    void fill();
    void fill(int r, int g, int b, int alpha);
    void render();
    void setFullscreen(bool fullscreen);
    void getWindowSize(int* x, int* y);
    SDL_Renderer* getRenderer() const;
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif
