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
    bool fxEnabled = true;
    bool inColorMenu = false;
    int targetScore = 15;
    bool gameOver = false;
    double labelTimer = 0.0;
    double scoreFlashTimer = 0.0;
    double ballExplosionTimer = 0.0;
    int pauseSelection = 0;
    int pendingMode = -1;
    int colorMenuPlayer = 0;
    int colorSelection = 0;
    int p1ColorIndex = 0;
    int p2ColorIndex = 1;
    bool p1RandomColor = false;
    bool p2RandomColor = false;

    bool inWinLoseScreen = false;
    double winLoseTimer = 0.0;
    bool winLoseShowPrompt = false;
    int lastWinner = 0; // 0 = none, 1 = P1, 2 = P2
    bool lastAiWin = false;

    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    Paddle* paddle1 = nullptr;
    Paddle* paddle2 = nullptr;
    Ball* ball = nullptr;

    int score1 = 0;
    int score2 = 0;
    int lastScore1 = 0;
    int lastScore2 = 0;

    float ballExplosionX = 0.0f;
    float ballExplosionY = 0.0f;

    Uint64 perfFreq = 0;
    Uint64 lastCounter = 0;
};