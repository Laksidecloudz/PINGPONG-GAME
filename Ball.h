#pragma once
#include <cstddef>

class Paddle;

class Ball {
public:
    Ball(float x, float y, float vx, float vy, int r);

    void reset(float cx, float cy);
    void update(double dt, int screenW, int screenH,
                const Paddle& p1, const Paddle& p2,
                int& score1, int& score2,
                int& health1, int& health2,
                float& boost1, float& boost2,
                bool battleMode,
                bool& shield1Held, bool& shield2Held);
    void render() const;

public:
    float x, y;
    float velX, velY;
    float trailBoost;
    float rallyEnergy;
    float leftImpactTimer;
    float rightImpactTimer;
    float topImpactTimer;
    float bottomImpactTimer;
    float lastScoreX;
    float lastScoreY;
    int radius;

    bool isPiercing = false;
    int wallBounceCount = 0;
};