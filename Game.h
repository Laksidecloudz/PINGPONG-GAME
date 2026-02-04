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
    struct LoseShard {
        float x;
        float y;
        float vx;
        float vy;
        float ax;
        float ay;
        float size;
        float angle;
        float angularVelocity;
        float life;
        float maxLife;
        float r;
        float g;
        float b;
        bool active;
    };

    struct LoseWisp {
        float x;
        float y;
        float vx;
        float vy;
        float life;
        float maxLife;
        float r;
        float g;
        float b;
        bool active;
    };

    struct OrbitBall {
        float angle;
        float radius;
        float speed;
        float size;
        float r;
        float g;
        float b;
    };

    static const int MaxLoseShards = 96;
    static const int MaxLoseWisps = 64;
    static const int MaxOrbitBalls = 10;

    LoseShard loseShards[MaxLoseShards];
    LoseWisp loseWisps[MaxLoseWisps];
    OrbitBall orbitBalls[MaxOrbitBalls];

    int loseShardCount = 0;
    int loseWispCount = 0;
    int orbitBallCount = 0;
    bool loseShatterActive = false;
    bool winDanceActive = false;
    double loseShatterTimer = 0.0;
    double winDanceTimer = 0.0;
    float winPaddleStartX = 0.0f;
    float winPaddleStartY = 0.0f;
    float winPaddleLandingX = 0.0f;
    float winPaddleLandingY = 0.0f;

    int width, height;
    bool isRunning = false;
    bool inStartScreen = true;
    bool inMainMenu = false;
    bool paused = false;
    bool singlePlayer = false;
    bool endlessMode = false;
    bool fxEnabled = true;

    enum AIDifficulty { AI_EASY = 0, AI_MEDIUM = 1, AI_HARD = 2 };
    AIDifficulty aiDifficulty = AI_MEDIUM;
    int playerSide = 1; // 1 = left paddle (P1), 2 = right paddle (P2)
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

    int health1 = 5;
    int health2 = 5;
    static const int MaxHealth = 5;
    bool koWin = false;

    float boostMeter1 = 0.0f;
    float boostMeter2 = 0.0f;
    bool boostActive1 = false;
    bool boostActive2 = false;
    bool autoBoostEnabled = false;
    static constexpr float BoostMultiplier = 1.6f;
    static constexpr float BoostDrainRate = 0.8f;
    static constexpr float BoostFillAmount = 0.25f;

    float ballExplosionX = 0.0f;
    float ballExplosionY = 0.0f;

    Uint64 perfFreq = 0;
    Uint64 lastCounter = 0;
};