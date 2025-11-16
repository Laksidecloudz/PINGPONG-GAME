#include "Paddle.h"
#include "SDL3/SDL_opengl.h"
#include <algorithm>

Paddle::Paddle(float x_, float y_, int w, int h, float speed_, SDL_Scancode up, SDL_Scancode down)
    : x(x_), y(y_), width(w), height(h), speed(speed_), upKey(up), downKey(down) {}

void Paddle::handleInput(const bool* keyState) {
    vy = 0.0f;
    if (keyState[upKey])   vy -= speed;
    if (keyState[downKey]) vy += speed;
}

void Paddle::move(double dt, int screenH) {
    y += vy * (float)dt;
    if (y < 0.0f) y = 0.0f;
    if (y > (float)(screenH - height)) y = (float)(screenH - height);
}

void Paddle::render() const {
    glColor4f(0.9f, 0.9f, 0.95f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}