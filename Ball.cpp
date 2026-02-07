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
      lastScoreX(x_), lastScoreY(y_),
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
    isPiercing = false;
    wallBounceCount = 0;
    // Randomize initial direction slightly
    float dirX = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
    float dirY = ((std::rand() % 100) / 100.0f) * 2.0f - 1.0f;
    float speed = 450.0f;
    velX = dirX * speed;
    velY = dirY * (speed * 0.4f);
}

void Ball::update(double dt, int screenW, int screenH,
                  const Paddle& p1, const Paddle& p2,
                  int& score1, int& score2,
                  int& health1, int& health2,
                  float& boost1, float& boost2,
                  bool battleMode,
                  bool shield1Active, bool shield2Active) {
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

    // Top/bottom walls - ball becomes piercing (red) on wall ricochet
    if (y - radius <= 0.0f) {
        y = (float)radius;
        velY = -velY;
        rallyEnergy += 0.15f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        topImpactTimer = 0.12f;
        if (battleMode) {
            isPiercing = true;
            wallBounceCount++;
        }
    } else if (y + radius >= (float)screenH) {
        y = (float)screenH - radius;
        velY = -velY;
        rallyEnergy += 0.15f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        bottomImpactTimer = 0.12f;
        if (battleMode) {
            isPiercing = true;
            wallBounceCount++;
        }
    }

    // Scoring: left/right bounds
    if (x + radius < 0.0f) {
        lastScoreX = 0.0f;
        lastScoreY = y;
        score2++;
        reset(screenW * 0.5f, screenH * 0.5f);
        return;
    } else if (x - radius > (float)screenW) {
        lastScoreX = (float)screenW;
        lastScoreY = y;
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
        
        // Rally speed boost: increase speed on each hit (capped at 900)
        float baseSpeed = 380.0f;
        float maxSpeed = 900.0f;
        float speedBoost = 1.0f + rallyEnergy * 0.4f; // Up to 40% boost at max energy
        speed = speed * 1.03f; // 3% increase per hit
        if (speed < baseSpeed) speed = baseSpeed;
        if (speed > maxSpeed) speed = maxSpeed;

        // Reflect to the right
        velX = std::fabs(speed * 0.95f);
        velY = hitPos * speed * 0.8f;

        trailBoost = 1.0f;
        rallyEnergy += 0.3f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        leftImpactTimer = 0.12f;

        // Deal damage if ball was piercing (red ricochet) - Battle Mode only
        // Shield blocks piercing damage and adds speed boost
        if (battleMode && isPiercing) {
            if (shield1Active) {
                // Shield deflect: no damage, speed boost
                float spd = std::sqrt(velX * velX + velY * velY);
                if (spd > 0.0f) {
                    float boosted = spd * 1.3f;
                    if (boosted > 900.0f) boosted = 900.0f;
                    float ratio = boosted / spd;
                    velX *= ratio;
                    velY *= ratio;
                }
            } else if (health1 > 0) {
                health1--;
            }
        }
        isPiercing = false;
        wallBounceCount = 0;

        // Fill boost meter for P1
        boost1 += 0.25f;
        if (boost1 > 1.0f) boost1 = 1.0f;

        // Nudge outside to avoid sticking
        x = p1.x + p1.width + radius + 0.5f;
    }

    // Right paddle
    if (aabbOverlap(x - radius, y - radius, radius * 2.0f, radius * 2.0f,
                    p2.x, p2.y, (float)p2.width, (float)p2.height)) {
        float paddleCenterY = p2.y + p2.height * 0.5f;
        float hitPos = (y - paddleCenterY) / (p2.height * 0.5f);
        float speed = std::sqrt(velX * velX + velY * velY);
        
        // Rally speed boost: increase speed on each hit (capped at 900)
        float baseSpeed = 380.0f;
        float maxSpeed = 900.0f;
        float speedBoost = 1.0f + rallyEnergy * 0.4f;
        speed = speed * 1.03f;
        if (speed < baseSpeed) speed = baseSpeed;
        if (speed > maxSpeed) speed = maxSpeed;

        // Reflect to the left
        velX = -std::fabs(speed * 0.95f);
        velY = hitPos * speed * 0.8f;

        // Nudge outside
        x = p2.x - radius - 0.5f;
        trailBoost = 1.0f;
        rallyEnergy += 0.3f;
        if (rallyEnergy > 1.0f) rallyEnergy = 1.0f;
        rightImpactTimer = 0.12f;

        // Deal damage if ball was piercing (red ricochet) - Battle Mode only
        // Shield blocks piercing damage and adds speed boost
        if (battleMode && isPiercing) {
            if (shield2Active) {
                // Shield deflect: no damage, speed boost
                float spd = std::sqrt(velX * velX + velY * velY);
                if (spd > 0.0f) {
                    float boosted = spd * 1.3f;
                    if (boosted > 900.0f) boosted = 900.0f;
                    float ratio = boosted / spd;
                    velX *= ratio;
                    velY *= ratio;
                }
            } else if (health2 > 0) {
                health2--;
            }
        }
        isPiercing = false;
        wallBounceCount = 0;

        // Fill boost meter for P2
        boost2 += 0.25f;
        if (boost2 > 1.0f) boost2 = 1.0f;
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

    // Color based on piercing state
    float frontR, frontG, frontB;
    float backR, backG, backB;
    float sideR, sideG, sideB;
    float topR, topG, topB;
    float bottomR, bottomG, bottomB;

    if (isPiercing) {
        // Red piercing ball
        frontR = 1.0f; frontG = 0.2f; frontB = 0.2f;
        backR = 0.6f; backG = 0.1f; backB = 0.1f;
        sideR = 0.95f; sideG = 0.15f; sideB = 0.15f;
        topR = 1.0f; topG = 0.3f; topB = 0.3f;
        bottomR = 0.5f; bottomG = 0.1f; bottomB = 0.1f;
    } else {
        // Normal white ball
        frontR = 0.98f; frontG = 0.98f; frontB = 1.0f;
        backR = 0.4f; backG = 0.4f; backB = 0.6f;
        sideR = 0.95f; sideG = 0.95f; sideB = 1.0f;
        topR = 1.0f; topG = 1.0f; topB = 1.0f;
        bottomR = 0.35f; bottomG = 0.35f; bottomB = 0.55f;
    }

    glBegin(GL_QUADS);
    // Front face
    glColor4f(frontR, frontG, frontB, 1.0f);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x0, y1, zFront);

    // Back face
    glColor4f(backR, backG, backB, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y1, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y0, zBack);

    // Left side
    glColor4f(sideR, sideG, sideB, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x0, y0, zFront);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x0, y1, zBack);

    // Right side
    glColor4f(sideR * 0.6f, sideG * 0.6f, sideB * 0.75f, 1.0f);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x1, y1, zFront);

    // Top
    glColor4f(topR, topG, topB, 1.0f);
    glVertex3f(x0, y0, zBack);
    glVertex3f(x1, y0, zBack);
    glVertex3f(x1, y0, zFront);
    glVertex3f(x0, y0, zFront);

    // Bottom
    glColor4f(bottomR, bottomG, bottomB, 1.0f);
    glVertex3f(x0, y1, zFront);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x1, y1, zBack);
    glVertex3f(x0, y1, zBack);
    glEnd();
}