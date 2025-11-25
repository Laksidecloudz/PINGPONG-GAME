#include "Game.h"
#include "Paddle.h"
#include "Ball.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

static void drawChar(float x, float y, float scale, char c) {
    float s = scale;
    auto segment = [x, y, s](float x1, float y1, float x2, float y2) {
        glVertex2f(x + x1 * s, y + y1 * s);
        glVertex2f(x + x2 * s, y + y2 * s);
    };

    glBegin(GL_LINES);
    switch (c) {
    case 'A':
        segment(0.0f, 1.0f, 0.5f, 0.0f);
        segment(1.0f, 1.0f, 0.5f, 0.0f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        break;
    case 'D':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 0.0f, 0.6f, 0.2f);
        segment(0.6f, 0.2f, 0.6f, 0.8f);
        segment(0.6f, 0.8f, 0.0f, 1.0f);
        break;
    case 'E':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 0.0f, 0.8f, 0.0f);
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        segment(0.0f, 1.0f, 0.8f, 1.0f);
        break;
    case 'F':
        // Vertical stem
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        // Top bar
        segment(0.0f, 0.0f, 0.8f, 0.0f);
        // Middle bar
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        break;
    case 'G':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.8f, 1.0f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.5f, 0.5f);
        break;
    case 'L':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 1.0f, 0.8f, 1.0f);
        break;
    case 'N':
        segment(0.0f, 1.0f, 0.0f, 0.0f);
        segment(0.0f, 0.0f, 1.0f, 1.0f);
        segment(1.0f, 1.0f, 1.0f, 0.0f);
        break;
    case 'O':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        break;
    case 'P':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 0.0f, 0.7f, 0.0f);
        segment(0.7f, 0.0f, 0.7f, 0.5f);
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        break;
    case 'S':
        segment(0.8f, 0.0f, 0.2f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.8f, 1.0f);
        segment(0.8f, 1.0f, 0.2f, 1.0f);
        break;
    case 'T':
        // Top bar
        segment(0.0f, 0.0f, 1.0f, 0.0f);
        // Vertical stem
        segment(0.5f, 0.0f, 0.5f, 1.0f);
        break;
    case 'R':
        // Left stem
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        // Top bar
        segment(0.0f, 0.0f, 0.8f, 0.0f);
        // Middle bar
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        // Right side of bowl
        segment(0.8f, 0.0f, 0.8f, 0.5f);
        // Diagonal leg
        segment(0.0f, 0.5f, 0.8f, 1.0f);
        break;
    case 'M':
        // Side stems
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(1.0f, 0.0f, 1.0f, 1.0f);
        // Peaks at top
        segment(0.0f, 0.0f, 0.5f, 0.5f);
        segment(1.0f, 0.0f, 0.5f, 0.5f);
        break;
    case 'V':
        segment(0.0f, 0.0f, 0.5f, 1.0f);
        segment(1.0f, 0.0f, 0.5f, 1.0f);
        break;
    case 'I':
        segment(0.5f, 0.0f, 0.5f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        break;
    case 'Q':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        segment(0.6f, 0.4f, 0.9f, 0.0f);
        break;
    case 'U':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(1.0f, 0.0f, 1.0f, 1.0f);
        segment(0.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 'W':
        segment(0.0f, 0.0f, 0.2f, 1.0f);
        segment(0.2f, 1.0f, 0.5f, 0.5f);
        segment(0.5f, 0.5f, 0.8f, 1.0f);
        segment(0.8f, 1.0f, 1.0f, 0.0f);
        break;
    case 'Y':
        segment(0.0f, 0.0f, 0.5f, 0.5f);
        segment(1.0f, 0.0f, 0.5f, 0.5f);
        segment(0.5f, 0.5f, 0.5f, 1.0f);
        break;
    case '0':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        break;
    case '1':
        segment(0.5f, 0.0f, 0.5f, 1.0f);
        segment(0.3f, 0.2f, 0.5f, 0.0f);
        segment(0.3f, 1.0f, 0.7f, 1.0f);
        break;
    case '2':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.8f, 0.0f, 0.8f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.2f, 0.5f, 0.2f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        break;
    case '3':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.8f, 0.0f, 0.8f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.8f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        break;
    case '4':
        segment(0.2f, 0.0f, 0.2f, 0.5f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        break;
    case '5':
        segment(0.8f, 0.0f, 0.2f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.8f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        break;
    case '6':
        segment(0.8f, 0.0f, 0.2f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.8f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        break;
    case '7':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.8f, 0.0f, 0.5f, 1.0f);
        break;
    case '8':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        break;
    case '9':
        segment(0.2f, 0.0f, 0.8f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 0.5f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
        break;
    case '/':
        segment(0.8f, 0.0f, 0.2f, 1.0f);
        break;
    case ':':
        segment(0.5f, 0.3f, 0.5f, 0.3f);
        segment(0.5f, 0.7f, 0.5f, 0.7f);
        break;
    case ' ':
        break;
    default:
        segment(0.0f, 0.0f, 1.0f, 1.0f);
        segment(1.0f, 0.0f, 0.0f, 1.0f);
        break;
    }
    glEnd();
}

static float measureText(float scale, const char* text) {
    float width = 0.0f;
    for (const char* p = text; *p; ++p) {
        char c = *p;
        if (c == ' ') {
            width += scale * 0.7f;
        } else if (c == '\n') {
            // We only use single-line text for width calculations here.
            break;
        } else {
            width += scale * 1.0f;
        }
    }
    return width;
}

static void drawText(float x, float y, float scale, const char* text) {
    float cursor = x;
    glLineWidth(2.0f);
    for (const char* p = text; *p; ++p) {
        char c = *p;
        if (c == ' ') {
            cursor += scale * 0.7f;
        } else if (c == '\n') {
            y += scale * 1.2f;
            cursor = x;
        } else {
            drawChar(cursor, y, scale, c);
            cursor += scale * 1.0f;
        }
    }
}

static const float kPaddleColorPresets[4][3] = {
    {0.2f, 0.95f, 1.0f},
    {0.3f, 1.0f, 0.4f},
    {1.0f, 0.8f, 0.2f},
    {1.0f, 0.3f, 0.9f}
};

static void applyPresetToPaddle(Paddle* paddle, int presetIndex) {
    if (!paddle) return;
    int idx = presetIndex;
    if (idx < 0) idx = 0;
    if (idx > 3) idx = 3;
    paddle->colorR = kPaddleColorPresets[idx][0];
    paddle->colorG = kPaddleColorPresets[idx][1];
    paddle->colorB = kPaddleColorPresets[idx][2];
}

Game::Game(int w, int h) : width(w), height(h) {}

bool Game::init() {
    Uint32 flags = SDL_INIT_VIDEO;
    bool ok = SDL_Init(flags);
    if (!ok) {
        const char* err = SDL_GetError();
        std::fprintf(stderr, "SDL_Init failed err='%s'\n", err ? err : "<null>");
        return false;
    }

    std::srand((unsigned int)SDL_GetPerformanceCounter());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "PingPong 3D (SDL3 + OpenGL)",
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::fprintf(stderr, "SDL_GL_CreateContext error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(1); // vsync

    glViewport(0, 0, width, height);
    glClearColor(0.06f, 0.07f, 0.10f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    resetProjection();

    // Create entities
    const int paddleW = 16;
    const int paddleH = 120;
    const float paddleSpeed = 600.0f;

    paddle1 = new Paddle(40.0f, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed, SDL_SCANCODE_W, SDL_SCANCODE_S);
    paddle2 = new Paddle(width - 40.0f - paddleW, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    applyPresetToPaddle(paddle1, p1ColorIndex);
    applyPresetToPaddle(paddle2, p2ColorIndex);

    ball = new Ball(width * 0.5f, height * 0.5f, 420.0f, 140.0f, 10);

    perfFreq = SDL_GetPerformanceFrequency();
    lastCounter = SDL_GetPerformanceCounter();
    isRunning = true;
    return true;
}

void Game::resetProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 2D ortho in screen space; we can still extend to 3D later
    glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            isRunning = false;
        } else if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            width = e.window.data1;
            height = e.window.data2;
            glViewport(0, 0, width, height);
            resetProjection();
        } else if (e.type == SDL_EVENT_KEY_DOWN) {
            SDL_Scancode sc = e.key.scancode;

            if (inStartScreen) {
                if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                    inStartScreen = false;
                    inMainMenu = true;
                    paused = true;
                    // Default to first competitive mode in the main menu
                    pauseSelection = 0;
                    labelTimer = 0.0;
                } else if (sc == SDL_SCANCODE_ESCAPE) {
                    isRunning = false;
                }
            } else {
                // Global ESC: open/close pause menu, or quit from the main menu
                if (sc == SDL_SCANCODE_ESCAPE) {
                    if (inColorMenu) {
                        inColorMenu = false;
                        pendingMode = -1;
                        colorMenuPlayer = 0;
                        colorSelection = 0;
                    } else if (inMainMenu) {
                        // From the opening main menu, ESC quits the game
                        isRunning = false;
                    } else {
                        paused = !paused;
                        if (paused) {
                            pauseSelection = 0; // RESUME when pausing in-game
                        }
                        labelTimer = 0.0;
                    }
                } else {
                    SDL_Keymod mods = SDL_GetModState();
                    bool ctrlDown = (mods & SDL_KMOD_CTRL) != 0;
                    if (ctrlDown && sc == SDL_SCANCODE_0) {
                        fxEnabled = !fxEnabled;
                    } else if (paused) {
                        if (inColorMenu) {
                            const int menuItems = 5;
                            if (sc == SDL_SCANCODE_UP) {
                                colorSelection = (colorSelection - 1 + menuItems) % menuItems;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                colorSelection = (colorSelection + 1) % menuItems;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (colorMenuPlayer == 0) {
                                    if (colorSelection == 4) {
                                        p1RandomColor = true;
                                    } else {
                                        p1RandomColor = false;
                                        p1ColorIndex = colorSelection;
                                    }
                                    colorMenuPlayer = 1;
                                    if (p2RandomColor) {
                                        colorSelection = 4;
                                    } else {
                                        if (p2ColorIndex < 0) p2ColorIndex = 0;
                                        if (p2ColorIndex > 3) p2ColorIndex = 3;
                                        colorSelection = p2ColorIndex;
                                    }
                                } else {
                                    if (colorSelection == 4) {
                                        p2RandomColor = true;
                                    } else {
                                        p2RandomColor = false;
                                        p2ColorIndex = colorSelection;
                                    }

                                    if (pendingMode == 0) {
                                        singlePlayer = true;
                                        endlessMode = false;
                                        targetScore = 15;
                                    } else if (pendingMode == 1) {
                                        singlePlayer = true;
                                        endlessMode = false;
                                        targetScore = 30;
                                    } else if (pendingMode == 2) {
                                        singlePlayer = false;
                                        endlessMode = false;
                                        targetScore = 15;
                                    } else if (pendingMode == 3) {
                                        singlePlayer = true;
                                        endlessMode = true;
                                        targetScore = 0;
                                    }

                                    int idx1 = p1RandomColor ? (std::rand() % 4) : p1ColorIndex;
                                    int idx2 = p2RandomColor ? (std::rand() % 4) : p2ColorIndex;
                                    applyPresetToPaddle(paddle1, idx1);
                                    applyPresetToPaddle(paddle2, idx2);

                                    gameOver = false;
                                    score1 = score2 = 0;
                                    paused = false;
                                    inMainMenu = false;
                                    inColorMenu = false;
                                    pendingMode = -1;
                                    colorMenuPlayer = 0;
                                    colorSelection = 0;
                                    labelTimer = 0.0;
                                }
                            }
                        } else {
                            // Pause/main menu navigation
                            const int menuItems = inMainMenu ? 5 : 7;
                            if (sc == SDL_SCANCODE_UP) {
                                pauseSelection = (pauseSelection - 1 + menuItems) % menuItems;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                pauseSelection = (pauseSelection + 1) % menuItems;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (inMainMenu) {
                                    if (pauseSelection == 0) {
                                        pendingMode = 0;
                                    } else if (pauseSelection == 1) {
                                        pendingMode = 1;
                                    } else if (pauseSelection == 2) {
                                        pendingMode = 2;
                                    } else if (pauseSelection == 3) {
                                        pendingMode = 3;
                                    } else if (pauseSelection == 4) {
                                        isRunning = false;
                                    }
                                } else {
                                    if (pauseSelection == 0) {
                                        paused = false;
                                    } else if (pauseSelection == 1) {
                                        pendingMode = 0;
                                    } else if (pauseSelection == 2) {
                                        pendingMode = 1;
                                    } else if (pauseSelection == 3) {
                                        pendingMode = 2;
                                    } else if (pauseSelection == 4) {
                                        pendingMode = 3;
                                    } else if (pauseSelection == 5) {
                                        fxEnabled = !fxEnabled;
                                    } else if (pauseSelection == 6) {
                                        isRunning = false;
                                    }
                                }

                                if (pendingMode >= 0 && paused) {
                                    inColorMenu = true;
                                    colorMenuPlayer = 0;
                                    if (p1RandomColor) {
                                        colorSelection = 4;
                                    } else {
                                        if (p1ColorIndex < 0) p1ColorIndex = 0;
                                        if (p1ColorIndex > 3) p1ColorIndex = 3;
                                        colorSelection = p1ColorIndex;
                                    }
                                }

                                labelTimer = 0.0;
                            }
                        }
                    } else {
                        if (sc == SDL_SCANCODE_P) {
                            paused = !paused;
                            if (paused) {
                                pauseSelection = 0;
                            }
                            labelTimer = 0.0;
                        }
                    }
                }
            }
        }
    }
}

void Game::update(double dt) {
    if (ballExplosionTimer > 0.0) {
        ballExplosionTimer -= dt;
        if (ballExplosionTimer < 0.0) ballExplosionTimer = 0.0;
    }

    if (inStartScreen || paused || gameOver) {
        return;
    }

    labelTimer += dt;
    if (scoreFlashTimer > 0.0) {
        scoreFlashTimer -= dt;
        if (scoreFlashTimer < 0.0) scoreFlashTimer = 0.0;
    }

    const bool* keys = SDL_GetKeyboardState(nullptr);
    paddle1->handleInput(keys);

    if (singlePlayer && ball) {
        float paddleCenterY = paddle2->y + paddle2->height * 0.5f;
        float targetY = ball->y;
        float diff = targetY - paddleCenterY;

        float maxSpeed = paddle2->speed * 0.8f;
        float desiredVy = diff * 3.0f;
        if (desiredVy > maxSpeed) desiredVy = maxSpeed;
        if (desiredVy < -maxSpeed) desiredVy = -maxSpeed;

        paddle2->setVerticalSpeed(desiredVy);
    } else {
        paddle2->handleInput(keys);
    }

    paddle1->move(dt, height);
    paddle2->move(dt, height);

    ball->update(dt, width, height, *paddle1, *paddle2, score1, score2);
    // Detect score changes to trigger a brief flash
    if (score1 != lastScore1 || score2 != lastScore2) {
        scoreFlashTimer = 0.3; // seconds
        ballExplosionTimer = 0.2;
        if (ball) {
            ballExplosionX = ball->lastScoreX;
            ballExplosionY = ball->lastScoreY;
        }
        lastScore1 = score1;
        lastScore2 = score2;
    }
    // Check competitive win condition if enabled
    if (!endlessMode && targetScore > 0) {
        if (score1 >= targetScore || score2 >= targetScore) {
            gameOver = true;
            paused = true;
            // Leave scores as-is so they can be displayed
        }
    }
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start screen: pure 2D UI
    if (inStartScreen) {
        glDisable(GL_DEPTH_TEST);
        resetProjection();
        glLoadIdentity();

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        const char* title = "PINGPONG 3D";
        float titleScale = 32.0f;
        float titleWidth = measureText(titleScale, title);
        float tx = (float)width * 0.5f - titleWidth * 0.5f;
        float ty = (float)height * 0.5f - titleScale * 1.5f;
        drawText(tx, ty, titleScale, title);

        const char* prompt = "PRESS ENTER";
        float promptScale = 18.0f;
        float promptWidth = measureText(promptScale, prompt);
        float px = (float)width * 0.5f - promptWidth * 0.5f;
        float py = ty + titleScale * 2.0f;
        drawText(px, py, promptScale, prompt);

        return;
    }

    // 3D world pass: perspective projection for center line, paddles, ball
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double zNear = 10.0;
    double zFar = 2000.0;
    double camZ = 800.0;
    double halfW = width * 0.5;
    double halfH = height * 0.5;
    double ymax = halfH * zNear / camZ;
    double ymin = -ymax;
    double xmax = halfW * zNear / camZ;
    double xmin = -xmax;
    glFrustum(xmin, xmax, ymax, ymin, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef((float)-halfW, (float)-halfH, (float)-camZ);
 
    float baseR = 0.02f;
    float baseG = 0.03f;
    float baseB = 0.06f;
    float baseR2 = 0.0f;
    float baseG2 = 0.0f;
    float baseB2 = 0.04f;
    float accentR = 0.3f;
    float accentG = 0.8f;
    float accentB = 1.0f;

    if (singlePlayer) {
        if (endlessMode) {
            accentR = 0.1f;
            accentG = 0.9f;
            accentB = 0.6f;
        } else if (targetScore >= 30) {
            accentR = 0.2f;
            accentG = 0.7f;
            accentB = 1.0f;
        } else {
            accentR = 0.4f;
            accentG = 0.7f;
            accentB = 1.0f;
        }
        baseR = 0.02f;
        baseG = 0.03f;
        baseB = 0.08f;
        baseR2 = 0.0f;
        baseG2 = 0.0f;
        baseB2 = 0.05f;
    } else {
        if (endlessMode) {
            accentR = 1.0f;
            accentG = 0.3f;
            accentB = 0.7f;
        } else {
            accentR = 1.0f;
            accentG = 0.4f;
            accentB = 0.3f;
        }
        baseR = 0.04f;
        baseG = 0.02f;
        baseB = 0.07f;
        baseR2 = 0.01f;
        baseG2 = 0.0f;
        baseB2 = 0.04f;
    }

    float rally = 0.0f;
    if (ball) {
        rally = ball->rallyEnergy;
        if (rally < 0.0f) rally = 0.0f;
        if (rally > 1.0f) rally = 1.0f;
    }

    float scorePulse = 0.0f;
    if (scoreFlashTimer > 0.0) {
        scorePulse = (float)(scoreFlashTimer / 0.3f);
        if (scorePulse > 1.0f) scorePulse = 1.0f;
    }

    float accentBoost = 1.0f + 0.4f * rally + 0.6f * scorePulse;
    if (accentBoost > 1.8f) accentBoost = 1.8f;
    accentR *= accentBoost;
    accentG *= accentBoost;
    accentB *= accentBoost;

    if (fxEnabled) {
        float zFloor = -6.0f;
        glBegin(GL_QUADS);
        glColor4f(baseR, baseG, baseB, 1.0f);
        glVertex3f(0.0f, 0.0f, zFloor);
        glVertex3f((float)width, 0.0f, zFloor);
        glColor4f(baseR2, baseG2, baseB2, 1.0f);
        glVertex3f((float)width, (float)height, zFloor);
        glVertex3f(0.0f, (float)height, zFloor);
        glEnd();
    }

    // 3D middle dashed line
    if (fxEnabled) {
        glColor4f(accentR, accentG, accentB, 0.3f);
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
    }
    for (int y = 0; y < height; y += 30) {
        float mx = width * 0.5f - 2.0f;
        float z = -5.0f;
        glBegin(GL_QUADS);
        glVertex3f(mx, (float)y, z);
        glVertex3f(mx + 4.0f, (float)y, z);
        glVertex3f(mx + 4.0f, (float)(y + 20), z);
        glVertex3f(mx, (float)(y + 20), z);
        glEnd();
    }

    if (fxEnabled) {
        float zShadow = -4.9f;

        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
        float p1x0 = paddle1->x;
        float p1x1 = paddle1->x + paddle1->width;
        float p1y0 = paddle1->y;
        float p1y1 = paddle1->y + paddle1->height;
        glVertex3f(p1x0, p1y0, zShadow);
        glVertex3f(p1x1, p1y0, zShadow);
        glVertex3f(p1x1, p1y1, zShadow);
        glVertex3f(p1x0, p1y1, zShadow);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
        float p2x0 = paddle2->x;
        float p2x1 = paddle2->x + paddle2->width;
        float p2y0 = paddle2->y;
        float p2y1 = paddle2->y + paddle2->height;
        glVertex3f(p2x0, p2y0, zShadow);
        glVertex3f(p2x1, p2y0, zShadow);
        glVertex3f(p2x1, p2y1, zShadow);
        glVertex3f(p2x0, p2y1, zShadow);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
        float bx0 = ball->x - ball->radius;
        float bx1 = ball->x + ball->radius;
        float by0 = ball->y - ball->radius;
        float by1 = ball->y + ball->radius;
        glVertex3f(bx0, by0, zShadow);
        glVertex3f(bx1, by0, zShadow);
        glVertex3f(bx1, by1, zShadow);
        glVertex3f(bx0, by1, zShadow);
        glEnd();
    }

    // 3D entities
    paddle1->render();
    paddle2->render();
    ball->render();

    // Switch to 2D for HUD and menus on top
    glDisable(GL_DEPTH_TEST);
    resetProjection();
    glLoadIdentity();

    if (fxEnabled) {
        float centerX = (float)width * 0.5f;
        float baseGlowWidth = 140.0f;
        float widthScale = 1.0f + 0.25f * rally + 0.5f * scorePulse;
        if (widthScale > 1.9f) widthScale = 1.9f;
        float centerGlowWidth = baseGlowWidth * widthScale;
        float centerGlowHalf = centerGlowWidth * 0.5f;
        float glowTop = 0.0f;
        float glowBottom = (float)height;

        float glowAlpha = 0.45f + 0.25f * rally + 0.4f * scorePulse;
        if (glowAlpha > 0.9f) glowAlpha = 0.9f;

        glBegin(GL_QUADS);
        glColor4f(accentR * 0.0f, accentG * 0.0f, accentB * 0.0f, 0.0f);
        glVertex2f(centerX - centerGlowHalf, glowTop);
        glVertex2f(centerX + centerGlowHalf, glowTop);
        glColor4f(accentR * 0.7f, accentG * 0.7f, accentB * 0.8f, glowAlpha);
        glVertex2f(centerX + centerGlowHalf, glowBottom);
        glVertex2f(centerX - centerGlowHalf, glowBottom);
        glEnd();

        if (ball) {
            float vx = ball->velX;
            float vy = ball->velY;
            float speed = std::sqrt(vx * vx + vy * vy);
            if (speed > 1.0f) {
                float nx = vx / speed;
                float ny = vy / speed;
                float boost = ball->trailBoost;
                if (boost < 0.0f) boost = 0.0f;
                if (boost > 1.0f) boost = 1.0f;
                float tailLength = (float)ball->radius * (5.0f + 3.0f * boost);
                float halfWidth = (float)ball->radius * 0.6f;

                float startOffset = (float)ball->radius * 1.0f;
                float hx = ball->x - nx * startOffset;
                float hy = ball->y - ny * startOffset;
                float tx = hx - nx * tailLength;
                float ty = hy - ny * tailLength;

                float px = -ny;
                float py = nx;

                float ax = hx + px * halfWidth;
                float ay = hy + py * halfWidth;
                float bx = hx - px * halfWidth;
                float by = hy - py * halfWidth;
                float cx = tx + px * halfWidth * 0.3f;
                float cy = ty + py * halfWidth * 0.3f;
                float dx = tx - px * halfWidth * 0.3f;
                float dy = ty - py * halfWidth * 0.3f;

                float headR = accentR * (0.9f + 0.3f * boost);
                float headG = accentG * (0.9f + 0.3f * boost);
                float headB = accentB * (0.9f + 0.3f * boost);
                float tailR = accentR * 0.4f;
                float tailG = accentG * 0.4f;
                float tailB = accentB * 0.4f;

                float headA = 0.5f + 0.25f * boost;
                if (headA > 0.8f) headA = 0.8f;

                glBegin(GL_QUADS);
                glColor4f(headR, headG, headB, headA);
                glVertex2f(ax, ay);
                glVertex2f(bx, by);
                glColor4f(tailR, tailG, tailB, 0.0f);
                glVertex2f(dx, dy);
                glVertex2f(cx, cy);
                glEnd();
            }
        }

        if (ballExplosionTimer > 0.0) {
            float t = 1.0f - (float)(ballExplosionTimer / 0.2f);
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            float cx = ballExplosionX;
            float cy = ballExplosionY;
            float radius = 20.0f + 130.0f * t;
            float alpha = 0.95f * (1.0f - t);
            float ringR = accentR * 1.2f;
            float ringG = accentG * 1.2f;
            float ringB = accentB * 1.2f;

            glBegin(GL_TRIANGLE_FAN);
            glColor4f(ringR, ringG, ringB, 0.0f);
            glVertex2f(cx, cy);
            int segments = 32;
            for (int i = 0; i <= segments; ++i) {
                float ang = (float)i / (float)segments * 6.2831853f;
                float px2 = cx + std::cos(ang) * radius;
                float py2 = cy + std::sin(ang) * radius;
                glColor4f(ringR, ringG, ringB, alpha);
                glVertex2f(px2, py2);
            }
            glEnd();

            glLineWidth(3.0f);
            glBegin(GL_LINES);
            int shardCount = 8;
            float shardBase = radius * 0.5f;
            float shardLen = radius * 0.7f;
            float shardAlpha = alpha * 1.2f;
            if (shardAlpha > 1.0f) shardAlpha = 1.0f;
            for (int i = 0; i < shardCount; ++i) {
                float ang = (float)i / (float)shardCount * 6.2831853f + 0.4f;
                float dirX = std::cos(ang);
                float dirY = std::sin(ang);
                float sx = cx + dirX * shardBase;
                float sy = cy + dirY * shardBase;
                float ex = cx + dirX * (shardBase + shardLen);
                float ey = cy + dirY * (shardBase + shardLen);
                glColor4f(ringR, ringG, ringB, shardAlpha);
                glVertex2f(sx, sy);
                glColor4f(ringR, ringG, ringB, 0.0f);
                glVertex2f(ex, ey);
            }
            glEnd();
        }
    }

    if (fxEnabled && ball) {
        float maxImpact = 0.12f;

        if (ball->leftImpactTimer > 0.0f) {
            float t = ball->leftImpactTimer / maxImpact;
            if (t > 1.0f) t = 1.0f;
            float alpha = 0.8f * t;
            float flashW = 20.0f;
            float flashH = 60.0f;
            float px = paddle1->x + paddle1->width;
            float py = ball->y;
            float x0 = px;
            float x1 = px + flashW;
            float y0 = py - flashH * 0.5f;
            float y1 = py + flashH * 0.5f;
            if (y0 < 0.0f) y0 = 0.0f;
            if (y1 > (float)height) y1 = (float)height;

            glBegin(GL_QUADS);
            glColor4f(accentR, accentG, accentB, alpha);
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glColor4f(accentR, accentG, accentB, 0.0f);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
            glEnd();
        }

        if (ball->rightImpactTimer > 0.0f) {
            float t = ball->rightImpactTimer / maxImpact;
            if (t > 1.0f) t = 1.0f;
            float alpha = 0.8f * t;
            float flashW = 20.0f;
            float flashH = 60.0f;
            float px = paddle2->x;
            float py = ball->y;
            float x0 = px - flashW;
            float x1 = px;
            float y0 = py - flashH * 0.5f;
            float y1 = py + flashH * 0.5f;
            if (y0 < 0.0f) y0 = 0.0f;
            if (y1 > (float)height) y1 = (float)height;

            glBegin(GL_QUADS);
            glColor4f(accentR, accentG, accentB, alpha);
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glColor4f(accentR, accentG, accentB, 0.0f);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
            glEnd();
        }

        if (ball->topImpactTimer > 0.0f) {
            float t = ball->topImpactTimer / maxImpact;
            if (t > 1.0f) t = 1.0f;
            float alpha = 0.7f * t;
            float h = 18.0f;

            glBegin(GL_QUADS);
            glColor4f(accentR, accentG, accentB, alpha);
            glVertex2f(0.0f, 0.0f);
            glVertex2f((float)width, 0.0f);
            glColor4f(accentR, accentG, accentB, 0.0f);
            glVertex2f((float)width, h);
            glVertex2f(0.0f, h);
            glEnd();
        }

        if (ball->bottomImpactTimer > 0.0f) {
            float t = ball->bottomImpactTimer / maxImpact;
            if (t > 1.0f) t = 1.0f;
            float alpha = 0.7f * t;
            float h = 18.0f;
            float y0 = (float)height - h;
            float y1 = (float)height;

            glBegin(GL_QUADS);
            glColor4f(accentR, accentG, accentB, 0.0f);
            glVertex2f(0.0f, y0);
            glVertex2f((float)width, y0);
            glColor4f(accentR, accentG, accentB, alpha);
            glVertex2f((float)width, y1);
            glVertex2f(0.0f, y1);
            glEnd();
        }
    }

    if (paused) {
        float alpha = inMainMenu ? 1.0f : 0.5f;  // opaque for main menu, translucent for pause
        glColor4f(0.0f, 0.0f, 0.0f, alpha);
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f((float)width, 0.0f);
        glVertex2f((float)width, (float)height);
        glVertex2f(0.0f, (float)height);
        glEnd();
    }

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Only draw HUD when not in the opening main menu
    if (!inMainMenu) {
        float topY = 30.0f;

        // Control hints
        if (labelTimer < 3.0) {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            drawText(40.0f, topY, 12.0f, "P1: W/S");
            drawText((float)width - 200.0f, topY, 12.0f, "P2: UP/DOWN");
        }

        // Scores centered at top, with a brief flash when score changes
        char scoreText[32];
        std::snprintf(scoreText, sizeof(scoreText), "%d : %d", score1, score2);

        float baseScoreScale = 20.0f;
        float scoreScale = baseScoreScale;

        if (scoreFlashTimer > 0.0) {
            float t = (float)(scoreFlashTimer / 0.3f);
            // Slight size bump and warm color
            scoreScale = baseScoreScale + 4.0f * t;
            glColor4f(1.0f, 1.0f, 0.4f, 1.0f);
        } else {
            float sr = accentR * 0.6f + 0.4f;
            float sg = accentG * 0.6f + 0.4f;
            float sb = accentB * 0.6f + 0.4f;
            glColor4f(sr, sg, sb, 1.0f);
        }

        float scoreWidth = measureText(scoreScale, scoreText);
        float scoreX = (float)width * 0.5f - scoreWidth * 0.5f;
        float scoreY = topY + 30.0f;
        drawText(scoreX, scoreY, scoreScale, scoreText);

        // Mode label under the score
        char modeText[64];

        if (singlePlayer) {
            if (endlessMode) {
                std::snprintf(modeText, sizeof(modeText), "ENDLESS MODE AI");
            } else if (targetScore == 15) {
                std::snprintf(modeText, sizeof(modeText), "FIRST TO 15 AI");
            } else if (targetScore == 30) {
                std::snprintf(modeText, sizeof(modeText), "FIRST TO 30 AI");
            } else if (targetScore > 0) {
                std::snprintf(modeText, sizeof(modeText), "FIRST TO %d AI", targetScore);
            } else {
                std::snprintf(modeText, sizeof(modeText), "VS AI");
            }
        } else {
            if (targetScore > 0) {
                std::snprintf(modeText, sizeof(modeText), "FIRST TO %d PVP", targetScore);
            } else if (endlessMode) {
                std::snprintf(modeText, sizeof(modeText), "ENDLESS MODE PVP");
            } else {
                std::snprintf(modeText, sizeof(modeText), "P1 VS P2");
            }
        }

        float modeScale = 12.0f;
        float modeWidth = measureText(modeScale, modeText);
        float modeX = (float)width * 0.5f - modeWidth * 0.5f;
        float modeY = scoreY + baseScoreScale * 1.4f;
        float mr = accentR * 0.8f + 0.2f;
        float mg = accentG * 0.8f + 0.2f;
        float mb = accentB * 0.8f + 0.2f;
        glColor4f(mr, mg, mb, 1.0f);
        drawText(modeX, modeY, modeScale, modeText);

        // Player labels near paddles
        if (labelTimer < 3.0) {
            float labelScale = 14.0f;
            float labelYOffset = 30.0f;

            float p1Width = measureText(labelScale, "P1");
            float p2Width = measureText(labelScale, "P2");

            float p1X = paddle1->x + paddle1->width * 0.5f - p1Width * 0.5f;
            float p2X = paddle2->x + paddle2->width * 0.5f - p2Width * 0.5f;

            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            drawText(p1X, paddle1->y - labelYOffset, labelScale, "P1");
            drawText(p2X, paddle2->y - labelYOffset, labelScale, "P2");
        }
    }

    if (paused) {
        if (inColorMenu) {
            const char* headerText = (colorMenuPlayer == 0) ? "P1 COLOR" : "P2 COLOR";
            float headerScale = 24.0f;
            float textWidth = measureText(headerScale, headerText);
            float x = (float)width * 0.5f - textWidth * 0.5f;
            float y = (float)height * 0.5f - headerScale * 0.5f - 60.0f;
            drawText(x, y, headerScale, headerText);

            const char* items[5] = {
                "CYAN",
                "GREEN",
                "GOLD",
                "MAGENTA",
                "RANDOM"
            };

            float menuScale = 18.0f;
            float startY = y + headerScale * 1.8f;

            for (int i = 0; i < 5; ++i) {
                float w = measureText(menuScale, items[i]);
                float ix = (float)width * 0.5f - w * 0.5f;
                float iy = startY + i * (menuScale * 1.4f);

                if (i == colorSelection) {
                    glColor4f(1.0f, 1.0f, 0.3f, 1.0f);
                } else {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                }

                drawText(ix, iy, menuScale, items[i]);
            }
        } else if (inMainMenu) {
            // Opening main menu: opaque background, no RESUME option
            const char* headerText = "MAIN MENU";
            float headerScale = 24.0f;
            float textWidth = measureText(headerScale, headerText);
            float x = (float)width * 0.5f - textWidth * 0.5f;
            float y = (float)height * 0.5f - headerScale * 0.5f - 60.0f;
            drawText(x, y, headerScale, headerText);

            const char* items[5] = {
                "FIRST TO 15",
                "FIRST TO 30",
                "P1 VS P2",
                "ENDLESS MODE",
                "QUIT"
            };

            float menuScale = 18.0f;
            float startY = y + headerScale * 1.8f;

            for (int i = 0; i < 5; ++i) {
                float w = measureText(menuScale, items[i]);
                float ix = (float)width * 0.5f - w * 0.5f;
                float iy = startY + i * (menuScale * 1.4f);

                if (i == pauseSelection) {
                    glColor4f(1.0f, 1.0f, 0.3f, 1.0f);
                } else {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                }

                drawText(ix, iy, menuScale, items[i]);
            }
        } else {
            // In-game pause / game-over menu: translucent background, includes RESUME
            const char* pausedText = gameOver ? "GAME OVER" : "PAUSED";
            float pausedScale = 24.0f;
            float textWidth = measureText(pausedScale, pausedText);
            float x = (float)width * 0.5f - textWidth * 0.5f;
            float y = (float)height * 0.5f - pausedScale * 0.5f - 60.0f;
            drawText(x, y, pausedScale, pausedText);

            if (gameOver) {
                // Show winner text when a target score has been reached
                const char* winnerText = nullptr;
                if (score1 > score2) {
                    winnerText = "PLAYER 1 WINS";
                } else if (score2 > score1) {
                    winnerText = "PLAYER 2 WINS";
                }
                if (winnerText) {
                    float ws = 18.0f;
                    float ww = measureText(ws, winnerText);
                    float wx = (float)width * 0.5f - ww * 0.5f;
                    float wy = y + pausedScale * 1.6f;
                    drawText(wx, wy, ws, winnerText);
                }
            }

            const char* items[7] = {
                "RESUME",
                "FIRST TO 15",
                "FIRST TO 30",
                "P1 VS P2",
                "ENDLESS MODE",
                nullptr,
                "QUIT"
            };

            float menuScale = 18.0f;
            float startY = y + pausedScale * (gameOver ? 2.4f : 1.8f);

            for (int i = 0; i < 7; ++i) {
                const char* label = items[i];
                char fxLabel[32];
                if (i == 5) {
                    std::snprintf(fxLabel, sizeof(fxLabel), fxEnabled ? "VISUAL FX: ON" : "VISUAL FX: OFF");
                    label = fxLabel;
                }

                float w = measureText(menuScale, label);
                float ix = (float)width * 0.5f - w * 0.5f;
                float iy = startY + i * (menuScale * 1.4f);

                if (i == pauseSelection) {
                    glColor4f(1.0f, 1.0f, 0.3f, 1.0f);
                } else {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                }

                drawText(ix, iy, menuScale, label);
            }
        }
    }
}

void Game::run() {
    while (isRunning) {
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = (double)(now - lastCounter) / (double)perfFreq;
        lastCounter = now;

        handleEvents();
        update(dt);
        render();
        SDL_GL_SwapWindow(window);
    }
}

void Game::clean() {
    delete ball;
    delete paddle1;
    delete paddle2;

    if (glContext) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
