#include "Paddle.h"
#include "SDL3/SDL_opengl.h"

Paddle::Paddle(float x_, float y_, int w, int h, float speed_, SDL_Scancode up, SDL_Scancode down)
    : x(x_), y(y_), width(w), height(h), speed(speed_), upKey(up), downKey(down) {}

void Paddle::handleInput(const bool* keyState) {
    vy = 0.0f;
    if (keyState[upKey])   vy -= speed;
    if (keyState[downKey]) vy += speed;
}

void Paddle::setVerticalSpeed(float v) {
    vy = v;
}

void Paddle::move(double dt, int screenH) {
    y += vy * (float)dt;
    if (y < 0.0f) y = 0.0f;
    if (y > (float)(screenH - height)) y = (float)(screenH - height);
}

void Paddle::render() const {
    float zFront = -10.0f;
    float zBack  = 0.0f;

    float x0 = x;
    float x1 = x + width;
    float y0 = y;
    float y1 = y + height;

    glBegin(GL_QUADS);
    // Front face
    glColor4f(0.95f, 0.95f, 1.0f, 1.0f);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x0, y1, zFront);

    // Back face
    glColor4f(0.35f, 0.35f, 0.5f, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y1, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y0, zBack);

    // Left side
    glColor4f(0.9f, 0.9f, 1.0f, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x0, y1, zBack);

    // Right side
    glColor4f(0.55f, 0.55f, 0.7f, 1.0f);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y1, zFront);

    // Top
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x0, y0, zFront);

    // Bottom
    glColor4f(0.3f, 0.3f, 0.45f, 1.0f);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x0, y1, zBack);
    glEnd();
}