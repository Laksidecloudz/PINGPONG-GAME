#pragma once
#include "SDL3/SDL.h"

class Paddle {
public:
    Paddle(float x, float y, int w, int h, float speed,
           SDL_Scancode up, SDL_Scancode down,
           SDL_Scancode left, SDL_Scancode right);

    void handleInput(const bool* keyState, bool freeMove = false);
    void setVerticalSpeed(float v);
    void setHorizontalSpeed(float v);
    void move(double dt, int screenH, bool freeMove = false, float xMin = 0.0f, float xMax = 0.0f);
    void render() const;

public:
    float x, y;
    int width, height;
    float speed;
    SDL_Scancode upKey, downKey, leftKey, rightKey;

    float colorR;
    float colorG;
    float colorB;

private:
    float vx = 0.0f;
    float vy = 0.0f;
};