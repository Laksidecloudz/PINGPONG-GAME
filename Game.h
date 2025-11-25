#pragma once
#include "SDL3/SDL.h"

class Paddle;
class Ball;

class Game {
public:
    Game(int w, int h);
    bool init();
    void run();
    void clean();

private:
    void handleEvents();
    void update(double dt);
    void render();
    void resetProjection();

private:
    int width, height;
    bool isRunning = false;
    bool inStartScreen = true;
    bool inMainMenu = false;
    bool paused = false;
    bool singlePlayer = false;
    bool endlessMode = false;
    int targetScore = 15;
    bool gameOver = false;
    double labelTimer = 0.0;
    double scoreFlashTimer = 0.0;
    int pauseSelection = 0;

    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    Paddle* paddle1 = nullptr;
    Paddle* paddle2 = nullptr;
    Ball* ball = nullptr;

    int score1 = 0;
    int score2 = 0;
    int lastScore1 = 0;
    int lastScore2 = 0;

    Uint64 perfFreq = 0;
    Uint64 lastCounter = 0;
};