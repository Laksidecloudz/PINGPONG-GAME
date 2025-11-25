#include "Ball.h"
#include "Paddle.h"
#include "SDL3/SDL_opengl.h"
#include <cmath>
#include <cstdlib>

static inline bool aabbOverlap(float ax, float ay, float aw, float ah,
                               float bx, float by, float bw, float bh) {
    return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

Ball::Ball(float x_, float y_, float vx, float vy, int r)
    : x(x_), y(y_), velX(vx), velY(vy), trailBoost(0.0f), rallyEnergy(0.0f),
      leftImpactTimer(0.0f), rightImpactTimer(0.0f), topImpactTimer(0.0f), bottomImpactTimer(0.0f),
      radius(r) {}

void Ball::reset(float cx, float cy) {
    x = cx;
    y = cy;
    trailBoost = 0.0f;
    rallyEnergy = 0.0f;
    leftImpactTimer = 0.0f;
    rightImpactTimer = 0.0f;
    topImpactTimer = 0.0f;
    bottomImpactTimer = 0.0f;
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

    if (trailBoost > 0.0f) {
        trailBoost -= (float)dt * 1.5f;
        if (trailBoost < 0.0f) trailBoost = 0.0f;
    }

    if (rallyEnergy > 0.0f) {
        rallyEnergy -= (float)dt * 0.3f;
        if (rallyEnergy < 0.0f) rallyEnergy = 0.0f;
    }

    if (leftImpactTimer > 0.0f) {
        leftImpactTimer -= (float)dt;
        if (leftImpactTimer < 0.0f) leftImpactTimer = 0.0f;
    }
    if (rightImpactTimer > 0.0f) {
        rightImpactTimer -= (float)dt;
        if (rightImpactTimer < 0.0f) rightImpactTimer = 0.0f;
    }
    if (topImpactTimer > 0.0f) {
        topImpactTimer -= (float)dt;
        if (topImpactTimer < 0.0f) topImpactTimer = 0.0f;
    }
    if (bottomImpactTimer > 0.0f) {
        bottomImpactTimer -= (float)dt;
        if (bottomImpactTimer < 0.0f) bottomImpactTimer = 0.0f;
    }

    // Top/bottom walls
    if (y - radius <= 0.0f) {
        y = (float)radius;
        velY = -velY;
        rallyEnergy += 0.15f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        topImpactTimer = 0.12f;
    } else if (y + radius >= (float)screenH) {
        y = (float)screenH - radius;
        velY = -velY;
        rallyEnergy += 0.15f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        bottomImpactTimer = 0.12f;
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

        trailBoost = 1.0f;
        rallyEnergy += 0.3f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        leftImpactTimer = 0.12f;

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
        trailBoost = 1.0f;
        rallyEnergy += 0.3f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        rightImpactTimer = 0.12f;
    }
}

void Ball::render() const {
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
    glColor4f(0.98f, 0.98f, 1.0f, 1.0f);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x0, y1, zFront);

    // Back face
    glColor4f(0.4f, 0.4f, 0.6f, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y1, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y0, zBack);

    // Left side
    glColor4f(0.95f, 0.95f, 1.0f, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x0, y1, zBack);

    // Right side
    glColor4f(0.55f, 0.55f, 0.75f, 1.0f);
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
    glColor4f(0.35f, 0.35f, 0.55f, 1.0f);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x0, y1, zBack);
    glEnd();
}