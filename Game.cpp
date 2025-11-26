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
    case 'C':
        segment(0.8f, 0.0f, 0.2f, 0.0f);
        segment(0.2f, 0.0f, 0.2f, 1.0f);
        segment(0.2f, 1.0f, 0.8f, 1.0f);
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

static void hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
    float hh = h * 6.0f;
    int sector = (int)hh;
    float f = hh - (float)sector;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    r = v;
    g = v;
    b = v;

    switch (sector % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    }
}

static void applyRandomVividColor(Paddle* paddle) {
    if (!paddle) return;

    float h = (float)(std::rand() % 360) / 360.0f;
    float r, g, b;
    hsvToRgb(h, 0.9f, 1.0f, r, g, b);
    paddle->colorR = r;
    paddle->colorG = g;
    paddle->colorB = b;
}

static float clamp01(float x) {
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

static float easeOutBounce(float x) {
    x = clamp01(x);
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (x < 1.0f / d1) {
        return n1 * x * x;
    } else if (x < 2.0f / d1) {
        x -= 1.5f / d1;
        return n1 * x * x + 0.75f;
    } else if (x < 2.5f / d1) {
        x -= 2.25f / d1;
        return n1 * x * x + 0.9375f;
    } else {
        x -= 2.625f / d1;
        return n1 * x * x + 0.984375f;
    }
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
                if (inWinLoseScreen) {
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER || sc == SDL_SCANCODE_ESCAPE) {
                        inWinLoseScreen = false;
                        winLoseShowPrompt = false;
                        winLoseTimer = 0.0;
                    }
                    return;
                }

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
                    } else if (gameOver) {
                        // From a completed match, ESC returns to the opening main menu
                        inMainMenu = true;
                        paused = true;
                        gameOver = false;
                        pauseSelection = 0;
                        labelTimer = 0.0;
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

                                    if (p1RandomColor) {
                                        applyRandomVividColor(paddle1);
                                    } else {
                                        applyPresetToPaddle(paddle1, p1ColorIndex);
                                    }

                                    if (p2RandomColor) {
                                        applyRandomVividColor(paddle2);
                                    } else {
                                        applyPresetToPaddle(paddle2, p2ColorIndex);
                                    }

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

    if (inWinLoseScreen) {
        winLoseTimer += dt;
        double delay = lastAiWin ? 3.0 : 1.5;
        if (!winLoseShowPrompt && winLoseTimer >= delay) {
            winLoseShowPrompt = true;
        }

        if (lastAiWin && loseShatterActive) {
            loseShatterTimer += dt;
            float slow = 0.4f;
            float step = (float)(dt * slow);

            for (int i = 0; i < loseShardCount; ++i) {
                LoseShard& s = loseShards[i];
                if (!s.active) continue;
                s.vx += s.ax * step;
                s.vy += s.ay * step;
                s.x += s.vx * step;
                s.y += s.vy * step;
                s.angle += s.angularVelocity * step;
                s.life += (float)dt;
                if (s.life >= s.maxLife) {
                    s.active = false;
                }
            }

            for (int i = 0; i < loseWispCount; ++i) {
                LoseWisp& w = loseWisps[i];
                if (!w.active) continue;
                w.x += w.vx * step;
                w.y += w.vy * step;
                w.life += (float)dt;
                if (w.life >= w.maxLife) {
                    w.active = false;
                }
            }
        }

        if (!lastAiWin && winDanceActive) {
            winDanceTimer += dt;
        }
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

            inWinLoseScreen = true;
            winLoseTimer = 0.0;
            winLoseShowPrompt = false;

            lastWinner = (score1 > score2) ? 1 : 2;
            lastAiWin = (singlePlayer && lastWinner == 2);
            loseShatterActive = false;
            winDanceActive = false;
            loseShardCount = 0;
            loseWispCount = 0;
            orbitBallCount = 0;
            loseShatterTimer = 0.0;
            winDanceTimer = 0.0;

            if (lastAiWin) {
                loseShatterActive = true;

                Paddle* losingPaddle = paddle1;
                if (losingPaddle) {
                    float px0 = losingPaddle->x;
                    float py0 = losingPaddle->y;
                    float px1 = px0 + losingPaddle->width;
                    float py1 = py0 + losingPaddle->height;

                    loseShardCount = MaxLoseShards;

                    for (int i = 0; i < loseShardCount; ++i) {
                        LoseShard& s = loseShards[i];
                        float rx = (float)std::rand() / (float)RAND_MAX;
                        float ry = (float)std::rand() / (float)RAND_MAX;
                        s.x = px0 + (px1 - px0) * rx;
                        s.y = py0 + (py1 - py0) * ry;

                        float dirX = (float)std::rand() / (float)RAND_MAX - 0.5f;
                        float dirY = (float)std::rand() / (float)RAND_MAX - 0.2f;
                        s.vx = dirX * 260.0f;
                        s.vy = -80.0f + dirY * 80.0f;
                        s.ax = 0.0f;
                        s.ay = 650.0f;

                        s.size = 6.0f + ((float)std::rand() / (float)RAND_MAX) * 14.0f;
                        s.angle = ((float)std::rand() / (float)RAND_MAX) * 6.2831853f;
                        s.angularVelocity = -4.0f + ((float)std::rand() / (float)RAND_MAX) * 8.0f;
                        s.life = 0.0f;
                        s.maxLife = 1.6f + ((float)std::rand() / (float)RAND_MAX) * 0.6f;
                        s.r = losingPaddle->colorR;
                        s.g = losingPaddle->colorG;
                        s.b = losingPaddle->colorB;
                        s.active = true;
                    }
                }

                loseWispCount = MaxLoseWisps / 2;
                for (int i = 0; i < loseWispCount; ++i) {
                    LoseWisp& w = loseWisps[i];
                    float t = (float)i / (float)(loseWispCount > 0 ? loseWispCount : 1);

                    float baseX;
                    float baseY;
                    if (paddle1) {
                        baseX = paddle1->x + paddle1->width * 0.5f;
                        baseY = paddle1->y + paddle1->height * 0.5f;
                    } else {
                        baseX = (float)width * 0.25f;
                        baseY = (float)height * 0.5f;
                    }

                    w.x = baseX + (t - 0.5f) * 80.0f;
                    w.y = baseY + 20.0f;
                    w.vx = ((float)std::rand() / (float)RAND_MAX - 0.5f) * 40.0f;
                    w.vy = -40.0f - ((float)std::rand() / (float)RAND_MAX) * 40.0f;
                    w.life = 0.0f;
                    w.maxLife = 2.0f + ((float)std::rand() / (float)RAND_MAX) * 1.0f;

                    float cool = 0.5f + 0.5f * t;
                    if (paddle1) {
                        w.r = paddle1->colorR * 0.5f * cool;
                        w.g = paddle1->colorG * 0.7f * cool;
                        w.b = paddle1->colorB;
                    } else {
                        w.r = 0.5f * cool;
                        w.g = 0.6f * cool;
                        w.b = 1.0f;
                    }
                    w.active = true;
                }
            } else {
                winDanceActive = true;
                winDanceTimer = 0.0;

                Paddle* winner = (lastWinner == 1) ? paddle1 : paddle2;
                if (winner) {
                    winPaddleStartX = winner->x + winner->width * 0.5f;
                    winPaddleStartY = winner->y + winner->height * 0.5f;
                } else {
                    winPaddleStartX = (float)width * 0.25f;
                    winPaddleStartY = (float)height * 0.5f;
                }
                winPaddleLandingX = (float)width * 0.5f;
                winPaddleLandingY = (float)height * 0.5f;

                orbitBallCount = MaxOrbitBalls;
                for (int i = 0; i < orbitBallCount; ++i) {
                    OrbitBall& ob = orbitBalls[i];
                    float t = (float)i / (float)orbitBallCount;
                    ob.angle = t * 6.2831853f;
                    ob.radius = 70.0f + 20.0f * std::sin(t * 6.2831853f);
                    ob.speed = 1.4f + 0.3f * ((float)std::rand() / (float)RAND_MAX);
                    ob.size = 6.0f + 4.0f * ((float)std::rand() / (float)RAND_MAX);

                    float h = t;
                    float r, g, b;
                    hsvToRgb(h, 0.9f, 1.0f, r, g, b);
                    ob.r = r;
                    ob.g = g;
                    ob.b = b;
                }
            }
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
    if (inWinLoseScreen && gameOver && lastAiWin) {
        if (paddle2) {
            paddle2->render();
        }
    } else if (inWinLoseScreen && gameOver && !lastAiWin && winDanceActive) {
        Paddle* winner = (lastWinner == 1) ? paddle1 : paddle2;
        Paddle* loser = (lastWinner == 1) ? paddle2 : paddle1;

        if (loser) {
            loser->render();
        }

        if (winner) {
            float origCx = winner->x + winner->width * 0.5f;
            float origCy = winner->y + winner->height * 0.5f;

            float totalJump = 2.0f;
            float t = (float)winDanceTimer;
            if (t > totalJump) t = totalJump;
            float u = clamp01(t / totalJump);

            float pathX = winPaddleStartX + (winPaddleLandingX - winPaddleStartX) * u;
            float pathY = winPaddleStartY + (winPaddleLandingY - winPaddleStartY) * u;

            float hops = 5.0f;
            float hopPhase = (float)winDanceTimer * hops * 3.1415926f * 0.5f;
            float hopAmount = std::fabs(std::sin(hopPhase));
            float hopScale = (1.0f - u);
            float hopOffset = hopAmount * 40.0f * hopScale;
            float centerY = pathY - hopOffset;

            float bounce = easeOutBounce(u);
            float squash = 1.0f - bounce;
            float scaleX = 1.0f + 0.35f * squash;
            float scaleY = 1.0f - 0.35f * squash;

            float offsetX = pathX - origCx;
            float offsetY = centerY - origCy;

            glPushMatrix();
            glTranslatef(offsetX, offsetY, 0.0f);
            glTranslatef(origCx, origCy, 0.0f);
            glScalef(scaleX, scaleY, 1.0f);
            glTranslatef(-origCx, -origCy, 0.0f);
            winner->render();
            glPopMatrix();
        }
    } else {
        if (paddle1) paddle1->render();
        if (paddle2) paddle2->render();
        if (ball) ball->render();
    }

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
            if (inWinLoseScreen && gameOver) {
                float centerX = (float)width * 0.5f;
                float centerY = (float)height * 0.5f;

                // Background tint for win/lose
                if (fxEnabled) {
                    glBegin(GL_QUADS);
                    if (lastAiWin) {
                        glColor4f(0.05f, 0.0f, 0.10f, 0.85f);
                        glVertex2f(0.0f, 0.0f);
                        glVertex2f((float)width, 0.0f);
                        glColor4f(0.2f, 0.0f, 0.0f, 0.95f);
                        glVertex2f((float)width, (float)height);
                        glVertex2f(0.0f, (float)height);
                    } else {
                        glColor4f(accentR * 0.25f, accentG * 0.25f, accentB * 0.35f, 0.6f);
                        glVertex2f(0.0f, 0.0f);
                        glVertex2f((float)width, 0.0f);
                        glColor4f(accentR * 0.05f, accentG * 0.05f, accentB * 0.1f, 0.9f);
                        glVertex2f((float)width, (float)height);
                        glVertex2f(0.0f, (float)height);
                    }
                    glEnd();
                }

                if (lastAiWin && loseShatterActive) {
                    for (int i = 0; i < loseShardCount; ++i) {
                        const LoseShard& s = loseShards[i];
                        if (!s.active) continue;
                        float tLife = (s.maxLife > 0.0f) ? (s.life / s.maxLife) : 1.0f;
                        float alpha = 1.0f - tLife;
                        if (alpha <= 0.0f) continue;
                        alpha *= alpha;

                        float half = s.size * 0.5f;
                        float cs = std::cos(s.angle);
                        float sn = std::sin(s.angle);

                        float x0 = -half;
                        float y0 = -half;
                        float x1 = half;
                        float y1 = half;

                        float vx0 = s.x + x0 * cs - y0 * sn;
                        float vy0 = s.y + x0 * sn + y0 * cs;
                        float vx1 = s.x + x1 * cs - y0 * sn;
                        float vy1 = s.y + x1 * sn + y0 * cs;
                        float vx2 = s.x + x1 * cs - y1 * sn;
                        float vy2 = s.y + x1 * sn + y1 * cs;
                        float vx3 = s.x + x0 * cs - y1 * sn;
                        float vy3 = s.y + x0 * sn + y1 * cs;

                        glBegin(GL_TRIANGLE_FAN);
                        glColor4f(s.r, s.g, s.b, alpha);
                        glVertex2f(vx0, vy0);
                        glVertex2f(vx1, vy1);
                        glVertex2f(vx2, vy2);
                        glVertex2f(vx3, vy3);
                        glEnd();
                    }

                    for (int i = 0; i < loseWispCount; ++i) {
                        const LoseWisp& w = loseWisps[i];
                        if (!w.active) continue;
                        float tLife = (w.maxLife > 0.0f) ? (w.life / w.maxLife) : 1.0f;
                        float alpha = (1.0f - tLife) * 0.7f;
                        if (alpha <= 0.0f) continue;

                        float width2 = 4.0f;
                        float height2 = 32.0f;
                        float x0 = w.x - width2 * 0.5f;
                        float x1 = w.x + width2 * 0.5f;
                        float y0 = w.y - height2;
                        float y1 = w.y;

                        glBegin(GL_QUADS);
                        glColor4f(w.r, w.g, w.b, 0.0f);
                        glVertex2f(x0, y1);
                        glVertex2f(x1, y1);
                        glColor4f(w.r, w.g, w.b, alpha);
                        glVertex2f(x1, y0);
                        glVertex2f(x0, y0);
                        glEnd();
                    }
                }

                // Radial glow behind title
                if (fxEnabled) {
                    float t = (float)winLoseTimer;
                    float pulse = 0.7f + 0.3f * std::sin(t * 3.5f);
                    float radius = (float)width * 0.25f + pulse * 30.0f;
                    float alpha = 0.9f;

                    float glowR = accentR * (lastAiWin ? 0.7f : 1.4f);
                    float glowG = accentG * (lastAiWin ? 0.3f : 1.4f);
                    float glowB = accentB * (lastAiWin ? 0.5f : 1.4f);

                    glBegin(GL_TRIANGLE_FAN);
                    glColor4f(glowR, glowG, glowB, alpha);
                    glVertex2f(centerX, centerY);
                    int segments = 40;
                    for (int i = 0; i <= segments; ++i) {
                        float ang = (float)i / (float)segments * 6.2831853f;
                        float px2 = centerX + std::cos(ang) * radius;
                        float py2 = centerY + std::sin(ang) * radius;
                        glColor4f(glowR, glowG, glowB, 0.0f);
                        glVertex2f(px2, py2);
                    }
                    glEnd();
                }

                // Determine title text
                const char* title = "GAME OVER";
                if (singlePlayer) {
                    if (lastAiWin) {
                        title = "YOU LOSE";
                    } else if (lastWinner == 1) {
                        title = "YOU WIN";
                    }
                } else {
                    if (lastWinner == 1) {
                        title = "PLAYER 1 WINS";
                    } else if (lastWinner == 2) {
                        title = "PLAYER 2 WINS";
                    }
                }

                float baseScale = lastAiWin ? 26.0f : 30.0f;
                float pulse = 1.0f + 0.08f * std::sin((float)winLoseTimer * 4.0f);
                float titleScale = baseScale * pulse;

                float titleWidth = measureText(titleScale, title);
                float tx = centerX - titleWidth * 0.5f;
                float ty = centerY - titleScale * 1.2f;

                // Drop shadow for pseudo-3D depth
                float shadowOffset = 3.0f;
                glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
                drawText(tx + shadowOffset, ty + shadowOffset, titleScale, title);

                // Main title color
                if (lastAiWin) {
                    glColor4f(accentR * 1.4f, accentG * 0.3f, accentB * 0.4f, 1.0f);
                } else {
                    glColor4f(accentR * 1.4f, accentG * 1.4f, accentB * 1.4f, 1.0f);
                }
                drawText(tx, ty, titleScale, title);

                // Final score under title
                char scoreText[32];
                std::snprintf(scoreText, sizeof(scoreText), "%d : %d", score1, score2);
                float scoreScale = 18.0f;
                float scoreWidth = measureText(scoreScale, scoreText);
                float sx = centerX - scoreWidth * 0.5f;
                float sy = ty + titleScale * 1.1f;
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                drawText(sx, sy, scoreScale, scoreText);

                if (!lastAiWin && winDanceActive && orbitBallCount > 0) {
                    float centerScoreX = centerX;
                    float centerScoreY = sy + scoreScale * 0.3f;

                    for (int i = 0; i < orbitBallCount; ++i) {
                        const OrbitBall& ob = orbitBalls[i];
                        float angle = ob.angle + ob.speed * (float)winLoseTimer;
                        float ca = std::cos(angle);
                        float sa = std::sin(angle);

                        float radius = ob.radius;
                        float bx = centerScoreX + ca * radius;
                        float by = centerScoreY + sa * radius * 0.6f;

                        float depth = 0.6f + 0.4f * (sa * 0.5f + 0.5f);
                        float size = ob.size * depth;
                        float alpha = 0.5f + 0.5f * depth;

                        float tailLen = size * 3.0f;
                        float txDirX = -sa;
                        float txDirY = ca * 0.6f;

                        float hx = bx;
                        float hy = by;
                        float tx2 = hx - txDirX * tailLen;
                        float ty2 = hy - txDirY * tailLen;

                        float px2 = -txDirY;
                        float py2 = txDirX;
                        float halfW = size * 0.4f;

                        float ax = hx + px2 * halfW;
                        float ay = hy + py2 * halfW;
                        float bx2 = hx - px2 * halfW;
                        float by2 = hy - py2 * halfW;
                        float cx2 = tx2 - px2 * halfW * 0.4f;
                        float cy2 = ty2 - py2 * halfW * 0.4f;
                        float dx2 = tx2 + px2 * halfW * 0.4f;
                        float dy2 = ty2 + py2 * halfW * 0.4f;

                        glBegin(GL_QUADS);
                        glColor4f(ob.r, ob.g, ob.b, alpha);
                        glVertex2f(ax, ay);
                        glVertex2f(bx2, by2);
                        glColor4f(ob.r, ob.g, ob.b, 0.0f);
                        glVertex2f(cx2, cy2);
                        glVertex2f(dx2, dy2);
                        glEnd();

                        glBegin(GL_TRIANGLE_FAN);
                        glColor4f(ob.r, ob.g, ob.b, alpha);
                        glVertex2f(bx, by);
                        int segments = 12;
                        for (int k = 0; k <= segments; ++k) {
                            float ang = (float)k / (float)segments * 6.2831853f;
                            float rx = bx + std::cos(ang) * size;
                            float ry = by + std::sin(ang) * size;
                            glColor4f(ob.r, ob.g, ob.b, 0.0f);
                            glVertex2f(rx, ry);
                        }
                        glEnd();
                    }
                }

                // AI loss countdown
                if (lastAiWin) {
                    float total = 3.0f;
                    if (winLoseTimer < total) {
                        int count = 3 - (int)winLoseTimer;
                        if (count < 1) count = 1;
                        char num[4];
                        std::snprintf(num, sizeof(num), "%d", count);

                        float numScale = 34.0f;
                        float numWidth = measureText(numScale, num);
                        float nx = centerX - numWidth * 0.5f;
                        float ny = sy + numScale * 1.4f;

                        float jitter = 1.5f * std::sin((float)winLoseTimer * 10.0f);
                        glColor4f(1.0f, 0.4f, 0.4f, 1.0f);
                        drawText(nx + jitter, ny, numScale, num);
                    }
                }

                // Prompt
                if (winLoseShowPrompt) {
                    const char* prompt = "PRESS ENTER TO CONTINUE";
                    float promptScale = 16.0f;
                    float promptWidth = measureText(promptScale, prompt);
                    float px = centerX - promptWidth * 0.5f;
                    float py = sy + 40.0f;

                    float blink = 0.5f + 0.5f * std::sin((float)winLoseTimer * 4.0f);
                    float pr = lastAiWin ? 0.8f : 1.0f;
                    float pg = lastAiWin ? 0.8f : 1.0f;
                    float pb = 1.0f;
                    glColor4f(pr, pg, pb, blink);
                    drawText(px, py, promptScale, prompt);
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
