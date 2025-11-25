#pragma once
#include <cstddef>

class Paddle;

class Ball {
public:
    Ball(float x, float y, float vx, float vy, int r);

    void reset(float cx, float cy);
    void update(double dt, int screenW, int screenH,
                const Paddle& p1, const Paddle& p2,
                int& score1, int& score2);
    void render() const;

public:
    float x, y;
    float velX, velY;
    float trailBoost;
    int radius;
};