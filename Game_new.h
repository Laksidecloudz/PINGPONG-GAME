#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "AudioManager_new.h"

class Paddle;
class Ball;

class Game {
public:
    Game(int w, int h);
    bool init();
    void run();
    void clean();

private:
    void handleEvents();
    void update(double dt);
    void render();
    void resetProjection();
