#include "Ball.h"
#include "Paddle.h"
#include "SDL3/SDL_opengl.h"
#include <cmath>
#include <algorithm>

static inline bool aabbOverlap(float ax, float ay, float aw, float ah,
                               float bx, float by, float bw, float bh) {
    return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

Ball::Ball(float x_, float y_, float vx, float vy, int r)
    : x(x_), y(y_), velX(vx), velY(vy), radius(r) {}

void Ball::reset(float cx, float cy) {
    x = cx;
    y = cy;
    // Randomize initial direction slightly
    float dirX = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
    float dirY = ((std::rand() % 100) / 100.0f) * 2.0f - 1.0f;
    float speed = 450.0f;
    velX = dirX * speed;
    velY = dirY * (speed * 0.4f);
}

void Ball::update(double dt, int screenW, int screenH,
                  const Paddle& p1, const Paddle& p2,
                  int& score1, int& score2) {
    x += velX * (float)dt;
    y += velY * (float)dt;

    // Top/bottom walls
    if (y - radius <= 0.0f) {
        y = (float)radius;
        velY = -velY;
    } else if (y + radius >= (float)screenH) {
        y = (float)screenH - radius;
        velY = -velY;
    }

    // Scoring: left/right bounds
    if (x + radius < 0.0f) {
        score2++;
        reset(screenW * 0.5f, screenH * 0.5f);
        return;
    } else if (x - radius > (float)screenW) {
        score1++;
        reset(screenW * 0.5f, screenH * 0.5f);
        return;
    }

    // Paddle collisions (approx using AABB vs circle as expanded AABB)
    // Left paddle
    if (aabbOverlap(x - radius, y - radius, radius * 2.0f, radius * 2.0f,
                    p1.x, p1.y, (float)p1.width, (float)p1.height)) {
        float paddleCenterY = p1.y + p1.height * 0.5f;
        float hitPos = (y - paddleCenterY) / (p1.height * 0.5f); // -1..1
        float speed = std::sqrt(velX * velX + velY * velY);
        if (speed < 380.0f) speed = 380.0f;

        // Reflect to the right
        velX = std::fabs(speed * 0.9f);
        velY = hitPos * speed;

        // Nudge outside to avoid sticking
        x = p1.x + p1.width + radius + 0.5f;
    }

    // Right paddle
    if (aabbOverlap(x - radius, y - radius, radius * 2.0f, radius * 2.0f,
                    p2.x, p2.y, (float)p2.width, (float)p2.height)) {
        float paddleCenterY = p2.y + p2.height * 0.5f;
        float hitPos = (y - paddleCenterY) / (p2.height * 0.5f);
        float speed = std::sqrt(velX * velX + velY * velY);
        if (speed < 380.0f) speed = 380.0f;

        // Reflect to the left
        velX = -std::fabs(speed * 0.9f);
        velY = hitPos * speed;

        // Nudge outside
        x = p2.x - radius - 0.5f;
    }
}

void Ball::render() const {
    glColor4f(0.95f, 0.95f, 1.0f, 1.0f);
    float left = x - radius;
    float top = y - radius;
    float size = (float)radius * 2.0f;

    float x0 = left;
    float x1 = left + size;
    float y0 = top;
    float y1 = top + size;
    float zFront = -8.0f;
    float zBack  = 0.0f;

    glBegin(GL_QUADS);
    // Front face
    glVertex3f(x0, y0, zFront);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x0, y1, zFront);

    // Back face
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y1, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y0, zBack);

    // Left side
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x0, y1, zBack);

    // Right side
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y1, zFront);

    // Top
    glVertex3f(x0, y0, zBack);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x0, y0, zFront);

    // Bottom
    glVertex3f(x0, y1, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x0, y1, zBack);
    glEnd();
}