#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "AudioManager_Fixed.h"  // Fixed audio system with real output

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
    int menuHitTest(float my) const;

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

    struct BoostParticle {
        float x, y;
        float vx, vy;
        float life;
        float maxLife;
        float size;
        float r, g, b;
        bool active;
    };

    static const int MaxBoostParticles = 48;
    static const int MaxLoseShards = 96;
    static const int MaxLoseWisps = 64;
    static const int MaxOrbitBalls = 10;

    BoostParticle boostParticles1[MaxBoostParticles];
    BoostParticle boostParticles2[MaxBoostParticles];
    int boostParticleCount1 = 0;
    int boostParticleCount2 = 0;
    float boostEmitTimer1 = 0.0f;
    float boostEmitTimer2 = 0.0f;

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
    bool paused = false;

    enum MenuScreen { MENU_NONE = 0, MENU_MAIN, MENU_PLAY, MENU_SETTINGS, MENU_PAUSE };
    MenuScreen currentMenu = MENU_NONE;
    bool hasActiveGame = false;
    double menuAnimTimer = 0.0;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    bool p1UseMouse = false;
    bool p2UseMouse = false;
    bool singlePlayer = false;
    bool endlessMode = false;
    bool fxEnabled = true;
    bool freeMovement = false;
    bool soundEnabled = true;
    int volumePercent = 75;  // 25, 50, 75, 100

    enum GameMode { MODE_CLASSIC = 0, MODE_BATTLE = 1 };
    GameMode gameMode = MODE_CLASSIC;

    enum AIDifficulty { AI_EASY = 0, AI_MEDIUM = 1, AI_HARD = 2 };
    AIDifficulty aiDifficulty = AI_MEDIUM;
    int playerSide = 1; // 1 = left paddle (P1), 2 = right paddle (P2)
    bool inColorMenu = false;
    int targetScore = 15;
    bool gameOver = false;
    double labelTimer = 0.0;
    double scoreFlashTimer = 0.0;
    double ballExplosionTimer = 0.0;
    int menuSelection = 0;
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

    int health1 = 10;
    int health2 = 10;
    int maxHealth = 10;
    static const int MaxHealthOptions[4];
    int maxHealthIndex = 1; // index into MaxHealthOptions (0=5, 1=10, 2=15, 3=20)
    bool koWin = false;

    float boostMeter1 = 0.0f;
    float boostMeter2 = 0.0f;
    bool boostActive1 = false;
    bool boostActive2 = false;
    bool boostActive1Prev = false;  // Track previous state for sound triggers
    bool boostActive2Prev = false;  // Track previous state for sound triggers
    bool autoBoostEnabled = false;
    static constexpr float BoostMultiplier = 1.6f;
    static constexpr float BoostDrainRate = 0.8f;
    static constexpr float BoostFillAmount = 0.25f;

    bool shieldEnabled = true;
    bool shieldHeld1 = false;
    bool shieldHeld2 = false;
    float shieldCooldown1 = 0.0f;  // fixed-mode: cooldown after consumption
    float shieldCooldown2 = 0.0f;
    static constexpr float ShieldCooldown = 3.0f;
    static constexpr float ShieldSpeedBoost = 1.3f;

    struct ShieldPickup {
        float x, y;
        float targetY;       // where it floats to after spawning
        float bobTimer;
        bool active;
    };
    ShieldPickup shieldPickup1 = {0, 0, 0, 0, false};
    ShieldPickup shieldPickup2 = {0, 0, 0, 0, false};

    float ballExplosionX = 0.0f;
    float ballExplosionY = 0.0f;

    Uint64 perfFreq = 0;
    Uint64 lastCounter = 0;

    // Splash screen
    bool inSplashScreen = true;
    double splashTimer = 0.0;
    double splashFadeInDuration = 1.0;   // seconds
    double splashHoldDuration = 2.0;     // seconds
    double splashFadeOutDuration = 1.0;  // seconds
    GLuint splashTexture = 0;
    int splashImageWidth = 0;
    int splashImageHeight = 0;
    bool splashTextureLoaded = false;

    bool loadSplashTexture(const char* filepath);
    void renderSplashScreen();
    void cleanupSplashTexture();
};