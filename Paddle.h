#pragma once
#include "SDL3/SDL.h"

class Paddle {
public:
    Paddle(float x, float y, int w, int h, float speed, SDL_Scancode up, SDL_Scancode down);

    void handleInput(const bool* keyState);
    void setVerticalSpeed(float v);
    void move(double dt, int screenH);
    void render() const;

public:
    float x, y;
    int width, height;
    float speed;
    SDL_Scancode upKey, downKey;

    float colorR;
    float colorG;
    float colorB;

private:
    float vy = 0.0f;
};