#include "Paddle.h"
#include "SDL3/SDL_opengl.h"

Paddle::Paddle(float x_, float y_, int w, int h, float speed_,
               SDL_Scancode up, SDL_Scancode down,
               SDL_Scancode left, SDL_Scancode right)
    : x(x_), y(y_), width(w), height(h), speed(speed_),
      upKey(up), downKey(down), leftKey(left), rightKey(right) {
    colorR = 0.95f;
    colorG = 0.95f;
    colorB = 1.0f;
}

void Paddle::handleInput(const bool* keyState, bool freeMove) {
    vy = 0.0f;
    vx = 0.0f;
    if (keyState[upKey])   vy -= speed;
    if (keyState[downKey]) vy += speed;
    if (freeMove) {
        if (keyState[leftKey])  vx -= speed;
        if (keyState[rightKey]) vx += speed;
    }
}

void Paddle::setVerticalSpeed(float v) {
    vy = v;
}

void Paddle::setHorizontalSpeed(float v) {
    vx = v;
}

void Paddle::move(double dt, int screenH, bool freeMove, float xMin, float xMax) {
    y += vy * (float)dt;
    if (y < 0.0f) y = 0.0f;
    if (y > (float)(screenH - height)) y = (float)(screenH - height);

    if (freeMove) {
        x += vx * (float)dt;
        if (x < xMin) x = xMin;
        if (x > xMax) x = xMax;
    }
}

void Paddle::render() const {
    float zFront = -10.0f;
    float zBack  = 0.0f;

    float x0 = x;
    float x1 = x + width;
    float y0 = y;
    float y1 = y + height;

    bool isLeftPaddle = (upKey == SDL_SCANCODE_W);

    float br = colorR;
    float bg = colorG;
    float bb = colorB;

    float dr = colorR * 0.4f;
    float dg = colorG * 0.4f;
    float db = colorB * 0.5f;

    float mr = colorR * 0.7f;
    float mg = colorG * 0.7f;
    float mb = colorB * 0.8f;

    glBegin(GL_QUADS);
    // Front face
    glColor4f(br, bg, bb, 1.0f);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x0, y1, zFront);

    // Back face
    glColor4f(dr, dg, db, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y1, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y0, zBack);

    // Left side
    if (isLeftPaddle) {
        // Outer side on the left paddle: slightly darker
        glColor4f(dr, dg, db, 1.0f);
    } else {
        // Inner side on the right paddle (toward center): brighter
        glColor4f(br, bg, bb, 1.0f);
    }
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x0, y1, zBack);

    // Right side
    if (isLeftPaddle) {
        // Inner side on the left paddle (toward center): brighter
        glColor4f(br, bg, bb, 1.0f);
    } else {
        // Outer side on the right paddle: slightly darker
        glColor4f(dr, dg, db, 1.0f);
    }
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y1, zFront);

    // Top
    glColor4f(mr, mg, mb, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x0, y0, zFront);

    // Bottom
    glColor4f(dr, dg, db, 1.0f);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x0, y1, zBack);
    glEnd();
}