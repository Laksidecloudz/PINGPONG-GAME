#include "Game.h"
#include "Paddle.h"
#include "Ball.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

const int Game::MaxHealthOptions[4] = { 5, 10, 15, 20 };

static inline bool aabbOverlap(float ax, float ay, float aw, float ah,
                               float bx, float by, float bw, float bh) {
    return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

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
    case 'B':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 0.0f, 0.7f, 0.0f);
        segment(0.7f, 0.0f, 0.8f, 0.15f);
        segment(0.8f, 0.15f, 0.8f, 0.35f);
        segment(0.8f, 0.35f, 0.7f, 0.5f);
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        segment(0.7f, 0.5f, 0.8f, 0.65f);
        segment(0.8f, 0.65f, 0.8f, 0.85f);
        segment(0.8f, 0.85f, 0.7f, 1.0f);
        segment(0.0f, 1.0f, 0.7f, 1.0f);
        break;
    case 'H':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.8f, 0.0f, 0.8f, 1.0f);
        segment(0.0f, 0.5f, 0.8f, 0.5f);
        break;
    case 'J':
        segment(0.8f, 0.0f, 0.8f, 0.85f);
        segment(0.8f, 0.85f, 0.5f, 1.0f);
        segment(0.5f, 1.0f, 0.2f, 0.85f);
        segment(0.3f, 0.0f, 0.8f, 0.0f);
        break;
    case 'K':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.8f, 0.0f, 0.0f, 0.5f);
        segment(0.0f, 0.5f, 0.8f, 1.0f);
        break;
    case 'X':
        segment(0.0f, 0.0f, 0.8f, 1.0f);
        segment(0.8f, 0.0f, 0.0f, 1.0f);
        break;
    case 'Z':
        segment(0.0f, 0.0f, 0.8f, 0.0f);
        segment(0.8f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 1.0f, 0.8f, 1.0f);
        break;
    case '.':
        segment(0.4f, 0.9f, 0.6f, 0.9f);
        segment(0.6f, 0.9f, 0.6f, 1.0f);
        segment(0.6f, 1.0f, 0.4f, 1.0f);
        segment(0.4f, 1.0f, 0.4f, 0.9f);
        break;
    case '!':
        segment(0.5f, 0.0f, 0.5f, 0.65f);
        segment(0.4f, 0.85f, 0.6f, 0.85f);
        segment(0.6f, 0.85f, 0.6f, 1.0f);
        segment(0.6f, 1.0f, 0.4f, 1.0f);
        segment(0.4f, 1.0f, 0.4f, 0.85f);
        break;
    case '>':
        segment(0.2f, 0.0f, 0.8f, 0.5f);
        segment(0.8f, 0.5f, 0.2f, 1.0f);
        break;
    case '<':
        segment(0.8f, 0.0f, 0.2f, 0.5f);
        segment(0.2f, 0.5f, 0.8f, 1.0f);
        break;
    case '-':
        segment(0.2f, 0.5f, 0.8f, 0.5f);
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

    paddle1 = new Paddle(40.0f, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed,
                         SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
    paddle2 = new Paddle(width - 40.0f - paddleW, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed,
                         SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);

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

int Game::menuHitTest(float my) const {
    float startY = 0.0f, spacing = 0.0f;
    int count = 0;

    if (inColorMenu) {
        float hs = 24.0f;
        float cy = (float)height * 0.5f - hs * 0.5f - 60.0f;
        startY = cy + hs * 1.8f;
        spacing = 18.0f * 1.4f;
        count = 5;
    } else if (currentMenu == MENU_MAIN || currentMenu == MENU_PLAY || currentMenu == MENU_SETTINGS) {
        float hs = (currentMenu == MENU_MAIN) ? 32.0f : 24.0f;
        float hy = (float)height * 0.25f;
        float sepY = hy + hs * 1.3f;
        startY = sepY + 20.0f;
        spacing = 18.0f * 1.6f;
        if (currentMenu == MENU_MAIN) count = hasActiveGame ? 4 : 3;
        else if (currentMenu == MENU_PLAY) count = 5;
        else if (currentMenu == MENU_SETTINGS) count = 11;
    } else if (currentMenu == MENU_PAUSE) {
        float hs = 28.0f;
        float hy = (float)height * 0.3f;
        float sepY = hy + hs * 1.3f;
        startY = sepY + 24.0f;
        spacing = 20.0f * 1.8f;
        count = 3;
    } else {
        return -1;
    }

    if (my < startY || count == 0) return -1;
    int idx = (int)((my - startY) / spacing);
    return (idx >= 0 && idx < count) ? idx : -1;
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
                    currentMenu = MENU_MAIN;
                    paused = true;
                    menuSelection = 0;
                    menuAnimTimer = 0.0;
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
                        // Transition to main menu after win/lose
                        currentMenu = MENU_MAIN;
                        gameOver = false;
                        menuSelection = 0;
                        menuAnimTimer = 0.0;
                    }
                    return;
                }

                // ESC: navigate back through menu hierarchy
                if (sc == SDL_SCANCODE_ESCAPE) {
                    if (inColorMenu) {
                        inColorMenu = false;
                        pendingMode = -1;
                        colorMenuPlayer = 0;
                        colorSelection = 0;
                    } else if (currentMenu == MENU_MAIN) {
                        isRunning = false;
                    } else if (currentMenu == MENU_PLAY) {
                        currentMenu = MENU_MAIN;
                        menuSelection = 0;
                        menuAnimTimer = 0.0;
                    } else if (currentMenu == MENU_SETTINGS) {
                        // Return to where we came from
                        currentMenu = hasActiveGame && !gameOver ? MENU_PAUSE : MENU_MAIN;
                        menuSelection = 0;
                        menuAnimTimer = 0.0;
                    } else if (currentMenu == MENU_PAUSE) {
                        // Resume game
                        paused = false;
                        currentMenu = MENU_NONE;
                    } else if (gameOver) {
                        currentMenu = MENU_MAIN;
                        paused = true;
                        gameOver = false;
                        menuSelection = 0;
                        menuAnimTimer = 0.0;
                        labelTimer = 0.0;
                    } else {
                        // In gameplay: open pause menu
                        paused = true;
                        currentMenu = MENU_PAUSE;
                        menuSelection = 0;
                        menuAnimTimer = 0.0;
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
                                    koWin = false;
                                    score1 = score2 = 0;
                                    health1 = health2 = maxHealth;
                                    boostMeter1 = boostMeter2 = 0.0f;
                                    boostActive1 = boostActive2 = false;
                                    shieldHeld1 = shieldHeld2 = false;
                                    shieldCooldown1 = shieldCooldown2 = 0.0f;
                                    shieldPickup1.active = shieldPickup2.active = false;
                                    boostParticleCount1 = boostParticleCount2 = 0;
                                    paddle1->x = 40.0f;
                                    paddle1->y = (float)(height - paddle1->height) * 0.5f;
                                    paddle2->x = (float)width - 40.0f - (float)paddle2->width;
                                    paddle2->y = (float)(height - paddle2->height) * 0.5f;
                                    paused = false;
                                    currentMenu = MENU_NONE;
                                    hasActiveGame = true;
                                    inColorMenu = false;
                                    pendingMode = -1;
                                    colorMenuPlayer = 0;
                                    colorSelection = 0;
                                    labelTimer = 0.0;
                                }
                            }
                        } else if (currentMenu == MENU_MAIN) {
                            // MAIN MENU: PLAY, [CONTINUE], SETTINGS, QUIT
                            int itemCount = hasActiveGame ? 4 : 3;
                            if (sc == SDL_SCANCODE_UP) {
                                menuSelection = (menuSelection - 1 + itemCount) % itemCount;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                menuSelection = (menuSelection + 1) % itemCount;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (hasActiveGame) {
                                    // 0=PLAY, 1=CONTINUE, 2=SETTINGS, 3=QUIT
                                    if (menuSelection == 0) {
                                        currentMenu = MENU_PLAY;
                                        menuSelection = 0;
                                        menuAnimTimer = 0.0;
                                    } else if (menuSelection == 1) {
                                        // Continue existing game
                                        paused = false;
                                        currentMenu = MENU_NONE;
                                    } else if (menuSelection == 2) {
                                        currentMenu = MENU_SETTINGS;
                                        menuSelection = 0;
                                        menuAnimTimer = 0.0;
                                    } else if (menuSelection == 3) {
                                        isRunning = false;
                                    }
                                } else {
                                    // 0=PLAY, 1=SETTINGS, 2=QUIT
                                    if (menuSelection == 0) {
                                        currentMenu = MENU_PLAY;
                                        menuSelection = 0;
                                        menuAnimTimer = 0.0;
                                    } else if (menuSelection == 1) {
                                        currentMenu = MENU_SETTINGS;
                                        menuSelection = 0;
                                        menuAnimTimer = 0.0;
                                    } else if (menuSelection == 2) {
                                        isRunning = false;
                                    }
                                }
                                labelTimer = 0.0;
                            }
                        } else if (currentMenu == MENU_PLAY) {
                            // PLAY: CLASSIC VS AI, CLASSIC PVP, BATTLE VS AI, BATTLE PVP, BACK
                            const int itemCount = 5;
                            if (sc == SDL_SCANCODE_UP) {
                                menuSelection = (menuSelection - 1 + itemCount) % itemCount;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                menuSelection = (menuSelection + 1) % itemCount;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (menuSelection == 4) {
                                    // BACK
                                    currentMenu = MENU_MAIN;
                                    menuSelection = 0;
                                    menuAnimTimer = 0.0;
                                } else {
                                    // 0=CLASSIC AI, 1=CLASSIC PVP, 2=BATTLE AI, 3=BATTLE PVP
                                    if (menuSelection == 0) {
                                        gameMode = MODE_CLASSIC;
                                        singlePlayer = true;
                                    } else if (menuSelection == 1) {
                                        gameMode = MODE_CLASSIC;
                                        singlePlayer = false;
                                    } else if (menuSelection == 2) {
                                        gameMode = MODE_BATTLE;
                                        singlePlayer = true;
                                    } else if (menuSelection == 3) {
                                        gameMode = MODE_BATTLE;
                                        singlePlayer = false;
                                    }
                                    // Apply current score target settings
                                    if (endlessMode) {
                                        targetScore = 0;
                                    }
                                    // Go to color selection
                                    pendingMode = menuSelection;
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
                        } else if (currentMenu == MENU_SETTINGS) {
                            // SETTINGS: SCORE, AI, SIDE, MOVEMENT, BOOST, SHIELD, HP, VISUAL FX, P1 INPUT, P2 INPUT, BACK
                            const int itemCount = 11;
                            if (sc == SDL_SCANCODE_UP) {
                                menuSelection = (menuSelection - 1 + itemCount) % itemCount;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                menuSelection = (menuSelection + 1) % itemCount;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (menuSelection == 0) {
                                    // Cycle score target: 15 → 30 → Endless → 15
                                    if (!endlessMode && targetScore == 15) {
                                        targetScore = 30;
                                    } else if (!endlessMode && targetScore == 30) {
                                        endlessMode = true;
                                        targetScore = 0;
                                    } else {
                                        endlessMode = false;
                                        targetScore = 15;
                                    }
                                } else if (menuSelection == 1) {
                                    aiDifficulty = (AIDifficulty)((aiDifficulty + 1) % 3);
                                } else if (menuSelection == 2) {
                                    playerSide = (playerSide == 1) ? 2 : 1;
                                } else if (menuSelection == 3) {
                                    freeMovement = !freeMovement;
                                    if (!freeMovement) {
                                        paddle1->x = 40.0f;
                                        paddle2->x = (float)width - 40.0f - (float)paddle2->width;
                                    }
                                } else if (menuSelection == 4) {
                                    autoBoostEnabled = !autoBoostEnabled;
                                } else if (menuSelection == 5) {
                                    shieldEnabled = !shieldEnabled;
                                    if (!shieldEnabled) {
                                        shieldHeld1 = shieldHeld2 = false;
                                        shieldCooldown1 = shieldCooldown2 = 0.0f;
                                        shieldPickup1.active = shieldPickup2.active = false;
                                    }
                                } else if (menuSelection == 6) {
                                    maxHealthIndex = (maxHealthIndex + 1) % 4;
                                    maxHealth = MaxHealthOptions[maxHealthIndex];
                                } else if (menuSelection == 7) {
                                    fxEnabled = !fxEnabled;
                                } else if (menuSelection == 8) {
                                    // Toggle P1 input (mutual exclusion with P2)
                                    p1UseMouse = !p1UseMouse;
                                    if (p1UseMouse) p2UseMouse = false;
                                } else if (menuSelection == 9) {
                                    // Toggle P2 input (mutual exclusion with P1)
                                    p2UseMouse = !p2UseMouse;
                                    if (p2UseMouse) p1UseMouse = false;
                                } else if (menuSelection == 10) {
                                    // BACK
                                    currentMenu = hasActiveGame && !gameOver ? MENU_PAUSE : MENU_MAIN;
                                    menuSelection = 0;
                                    menuAnimTimer = 0.0;
                                }
                                labelTimer = 0.0;
                            }
                        } else if (currentMenu == MENU_PAUSE) {
                            // PAUSE: RESUME, SETTINGS, MAIN MENU
                            const int itemCount = 3;
                            if (sc == SDL_SCANCODE_UP) {
                                menuSelection = (menuSelection - 1 + itemCount) % itemCount;
                            } else if (sc == SDL_SCANCODE_DOWN) {
                                menuSelection = (menuSelection + 1) % itemCount;
                            } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                                if (menuSelection == 0) {
                                    // Resume
                                    paused = false;
                                    currentMenu = MENU_NONE;
                                } else if (menuSelection == 1) {
                                    // Settings
                                    currentMenu = MENU_SETTINGS;
                                    menuSelection = 0;
                                    menuAnimTimer = 0.0;
                                } else if (menuSelection == 2) {
                                    // Main Menu
                                    currentMenu = MENU_MAIN;
                                    gameOver = false;
                                    menuSelection = 0;
                                    menuAnimTimer = 0.0;
                                }
                                labelTimer = 0.0;
                            }
                        }
                    } else {
                        if (sc == SDL_SCANCODE_P) {
                            paused = true;
                            currentMenu = MENU_PAUSE;
                            menuSelection = 0;
                            menuAnimTimer = 0.0;
                            labelTimer = 0.0;
                        }
                    }
                }
            }
        } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
            mouseX = e.motion.x;
            mouseY = e.motion.y;
            // Update menu hover from mouse position
            if (paused && !inStartScreen && !inWinLoseScreen) {
                bool inXRange = (mouseX > (float)width * 0.1f && mouseX < (float)width * 0.9f);
                if (inXRange) {
                    int hover = menuHitTest(mouseY);
                    if (hover >= 0) {
                        if (inColorMenu) {
                            colorSelection = hover;
                        } else {
                            menuSelection = hover;
                        }
                    }
                }
            }
        } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            mouseX = e.button.x;
            mouseY = e.button.y;
            if (inStartScreen) {
                inStartScreen = false;
                currentMenu = MENU_MAIN;
                paused = true;
                menuSelection = 0;
                menuAnimTimer = 0.0;
                labelTimer = 0.0;
            } else if (inWinLoseScreen) {
                inWinLoseScreen = false;
                winLoseShowPrompt = false;
                winLoseTimer = 0.0;
                currentMenu = MENU_MAIN;
                gameOver = false;
                menuSelection = 0;
                menuAnimTimer = 0.0;
            } else if (paused) {
                bool inXRange = (mouseX > (float)width * 0.1f && mouseX < (float)width * 0.9f);
                int hover = inXRange ? menuHitTest(mouseY) : -1;
                if (hover >= 0) {
                    if (inColorMenu) {
                        colorSelection = hover;
                    } else {
                        menuSelection = hover;
                    }
                    // Simulate Enter keypress to trigger the selection action
                    SDL_Event synth = {};
                    synth.type = SDL_EVENT_KEY_DOWN;
                    synth.key.scancode = SDL_SCANCODE_RETURN;
                    SDL_PushEvent(&synth);
                }
            }
        }
    }
}

void Game::update(double dt) {
    menuAnimTimer += dt;

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
    
    // Boost activation: P1 = Shift, P2 = RCtrl
    bool p1WantsBoost = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
    bool p2WantsBoost = keys[SDL_SCANCODE_RCTRL];
    
    // Handle boost for P1
    if (autoBoostEnabled) {
        // Auto-boost: activate automatically when meter > 0
        boostActive1 = (boostMeter1 > 0.0f);
    } else {
        // Manual boost: hold key while having meter
        boostActive1 = (p1WantsBoost && boostMeter1 > 0.0f);
    }
    
    // Drain boost meter while active
    if (boostActive1) {
        boostMeter1 -= BoostDrainRate * (float)dt;
        if (boostMeter1 <= 0.0f) {
            boostMeter1 = 0.0f;
            boostActive1 = false;
        }
    }
    
    // Handle boost for P2 (only in 2-player mode)
    if (!singlePlayer) {
        if (autoBoostEnabled) {
            boostActive2 = (boostMeter2 > 0.0f);
        } else {
            boostActive2 = (p2WantsBoost && boostMeter2 > 0.0f);
        }
        
        if (boostActive2) {
            boostMeter2 -= BoostDrainRate * (float)dt;
            if (boostMeter2 <= 0.0f) {
                boostMeter2 = 0.0f;
                boostActive2 = false;
            }
        }
    }
    
    // Shield system (Battle Mode only when enabled)
    if (shieldEnabled && gameMode == MODE_BATTLE && ball) {
        float fdt = (float)dt;

        // Tick down cooldowns (used in fixed movement mode)
        if (shieldCooldown1 > 0.0f) {
            shieldCooldown1 -= fdt;
            if (shieldCooldown1 < 0.0f) shieldCooldown1 = 0.0f;
        }
        if (shieldCooldown2 > 0.0f) {
            shieldCooldown2 -= fdt;
            if (shieldCooldown2 < 0.0f) shieldCooldown2 = 0.0f;
        }

        if (freeMovement) {
            // --- COLLECTIBLE SHIELD MODE ---
            // Spawn pickups when ball is piercing, no active pickup, player doesn't hold one
            float pickupSize = 18.0f;
            float floatInset = 60.0f; // how far from wall edge the pickup floats to

            auto spawnPickup = [&](ShieldPickup& pickup, bool held, float xMin, float xMax) {
                if (pickup.active || held || !ball->isPiercing) return;
                pickup.x = xMin + ((float)(std::rand() % 100) / 100.0f) * (xMax - xMin);
                bool fromTop = (std::rand() % 2 == 0);
                pickup.y = fromTop ? -pickupSize : (float)height + pickupSize;
                pickup.targetY = fromTop ? floatInset : ((float)height - floatInset);
                pickup.bobTimer = 0.0f;
                pickup.active = true;
            };

            float p1XMin = 0.0f;
            float p1XMax = (float)width * 0.45f - pickupSize;
            float p2XMin = (float)width * 0.55f;
            float p2XMax = (float)width - pickupSize;

            spawnPickup(shieldPickup1, shieldHeld1, p1XMin, p1XMax);
            spawnPickup(shieldPickup2, shieldHeld2, p2XMin, p2XMax);

            // Animate pickups sliding in and bobbing
            auto updatePickup = [&](ShieldPickup& pickup) {
                if (!pickup.active) return;
                // Slide toward target Y
                float dy = pickup.targetY - pickup.y;
                float slideSpeed = 200.0f;
                if (std::fabs(dy) > 1.0f) {
                    float step = (dy > 0 ? 1.0f : -1.0f) * slideSpeed * fdt;
                    if (std::fabs(step) > std::fabs(dy)) step = dy;
                    pickup.y += step;
                }
                pickup.bobTimer += fdt;
            };
            updatePickup(shieldPickup1);
            updatePickup(shieldPickup2);

            // Despawn pickups if ball is no longer piercing
            if (!ball->isPiercing) {
                shieldPickup1.active = false;
                shieldPickup2.active = false;
            }

            // Check paddle-pickup collection
            auto checkCollect = [&](ShieldPickup& pickup, Paddle* pad, bool& held) {
                if (!pickup.active || !pad || held) return;
                float px0 = pad->x;
                float py0 = pad->y;
                float pw = (float)pad->width;
                float ph = (float)pad->height;
                float sx0 = pickup.x - pickupSize * 0.5f;
                float sy0 = pickup.y - pickupSize * 0.5f;
                if (aabbOverlap(px0, py0, pw, ph, sx0, sy0, pickupSize, pickupSize)) {
                    held = true;
                    pickup.active = false;
                }
            };
            checkCollect(shieldPickup1, paddle1, shieldHeld1);
            checkCollect(shieldPickup2, paddle2, shieldHeld2);
        } else {
            // --- FIXED MOVEMENT: KEY-ACTIVATED ONE-USE SHIELD ---
            if (singlePlayer) {
                // Player activates shield
                bool& pHeld = (playerSide == 1) ? shieldHeld1 : shieldHeld2;
                float& pCooldown = (playerSide == 1) ? shieldCooldown1 : shieldCooldown2;
                if (keys[SDL_SCANCODE_SPACE] && !pHeld && pCooldown <= 0.0f) {
                    pHeld = true;
                }

                // AI activates shield
                bool& aiHeld = (playerSide == 1) ? shieldHeld2 : shieldHeld1;
                float& aiCooldown = (playerSide == 1) ? shieldCooldown2 : shieldCooldown1;
                Paddle* aiPaddle = (playerSide == 1) ? paddle2 : paddle1;
                if (!aiHeld && aiCooldown <= 0.0f && ball->isPiercing) {
                    bool ballTowardAI = (playerSide == 1) ? (ball->velX > 0) : (ball->velX < 0);
                    if (ballTowardAI) {
                        float dist = std::fabs(ball->x - (aiPaddle->x + aiPaddle->width * 0.5f));
                        bool activate = false;
                        switch (aiDifficulty) {
                            case AI_EASY: activate = false; break;
                            case AI_MEDIUM: activate = (dist < 200.0f && ball->wallBounceCount >= 2); break;
                            case AI_HARD: activate = (dist < 300.0f); break;
                        }
                        if (activate) aiHeld = true;
                    }
                }
            } else {
                // PvP: P1 = Space, P2 = RAlt
                if (keys[SDL_SCANCODE_SPACE] && !shieldHeld1 && shieldCooldown1 <= 0.0f) {
                    shieldHeld1 = true;
                }
                if (keys[SDL_SCANCODE_RALT] && !shieldHeld2 && shieldCooldown2 <= 0.0f) {
                    shieldHeld2 = true;
                }
            }
        }
    }

    // Apply boost multiplier to paddle speeds temporarily
    float originalSpeed1 = paddle1->speed;
    float originalSpeed2 = paddle2->speed;
    if (boostActive1) paddle1->speed *= BoostMultiplier;
    if (boostActive2) paddle2->speed *= BoostMultiplier;
    
    if (singlePlayer && ball) {
        // Determine which paddle is player-controlled and which is AI
        Paddle* playerPaddle = (playerSide == 1) ? paddle1 : paddle2;
        Paddle* aiPaddle = (playerSide == 1) ? paddle2 : paddle1;
        
        // Player input (keyboard or mouse)
        if (p1UseMouse) {
            float targetY = mouseY - playerPaddle->height * 0.5f;
            if (targetY < 0.0f) targetY = 0.0f;
            if (targetY > (float)(height - playerPaddle->height)) targetY = (float)(height - playerPaddle->height);
            playerPaddle->y = targetY;
            playerPaddle->setVerticalSpeed(0.0f);
            if (freeMovement) {
                float pMinX = (playerSide == 1) ? 0.0f : (float)width * 0.55f;
                float pMaxX = (playerSide == 1) ? ((float)width * 0.45f - (float)playerPaddle->width) : ((float)width - (float)playerPaddle->width);
                float targetX = mouseX - playerPaddle->width * 0.5f;
                if (targetX < pMinX) targetX = pMinX;
                if (targetX > pMaxX) targetX = pMaxX;
                playerPaddle->x = targetX;
                playerPaddle->setHorizontalSpeed(0.0f);
            }
        } else {
            playerPaddle->handleInput(keys, freeMovement);
        }
        
        // AI control
        float paddleCenterY = aiPaddle->y + aiPaddle->height * 0.5f;
        int aiHealth = (playerSide == 1) ? health2 : health1;
        
        // Difficulty-based AI parameters
        float speedMult, reactionMult, predictionBlend;
        switch (aiDifficulty) {
            case AI_EASY:
                speedMult = 0.55f;
                reactionMult = 2.0f;
                predictionBlend = 0.4f;
                break;
            case AI_MEDIUM:
                speedMult = 0.75f;
                reactionMult = 3.5f;
                predictionBlend = 0.75f;
                break;
            case AI_HARD:
            default:
                speedMult = 0.95f;
                reactionMult = 5.0f;
                predictionBlend = 1.0f;
                break;
        }

        // Predictive AI: calculate where ball will be when it reaches AI paddle
        float targetY = ball->y;
        bool ballMovingTowardAI = (playerSide == 1) ? (ball->velX > 0) : (ball->velX < 0);
        if (ballMovingTowardAI) {
            float timeToReach = std::fabs((aiPaddle->x - ball->x) / ball->velX);
            if (timeToReach > 0 && timeToReach < 2.0f) {
                float predictedY = ball->y + ball->velY * timeToReach;
                // Handle wall bounces in prediction
                float screenH = (float)height;
                while (predictedY < 0 || predictedY > screenH) {
                    if (predictedY < 0) predictedY = -predictedY;
                    else if (predictedY > screenH) predictedY = 2 * screenH - predictedY;
                }
                // Blend between current ball Y and predicted Y based on difficulty
                targetY = ball->y * (1.0f - predictionBlend) + predictedY * predictionBlend;
            }
        } else {
            // Ball moving away: drift toward ready position (screen center)
            float readyY = (float)height * 0.5f;
            float driftBlend = 0.0f;
            switch (aiDifficulty) {
                case AI_EASY:   driftBlend = 0.15f; break;
                case AI_MEDIUM: driftBlend = 0.35f; break;
                case AI_HARD:   driftBlend = 0.55f; break;
            }
            targetY = ball->y * (1.0f - driftBlend) + readyY * driftBlend;
        }

        // Red ball avoidance in Battle Mode
        bool shouldDodge = false;
        if (gameMode == MODE_BATTLE && ball->isPiercing && ballMovingTowardAI) {
            switch (aiDifficulty) {
                case AI_EASY:
                    // Easy AI doesn't understand red ball danger
                    shouldDodge = false;
                    break;
                case AI_MEDIUM:
                    // Medium AI dodges when health critical or many wall bounces
                    shouldDodge = (aiHealth <= 2) || (ball->wallBounceCount >= 3);
                    break;
                case AI_HARD:
                    // Hard AI always dodges red balls
                    shouldDodge = true;
                    break;
            }
        }

        if (shouldDodge) {
            // Move away from predicted ball position
            float dodgeDist = (float)aiPaddle->height * 1.5f;
            float screenH = (float)height;
            // Dodge toward whichever side has more room
            if (targetY > screenH * 0.5f) {
                targetY -= dodgeDist;
            } else {
                targetY += dodgeDist;
            }
            float halfH = (float)aiPaddle->height * 0.5f;
            if (targetY < halfH) targetY = halfH;
            if (targetY > screenH - halfH) targetY = screenH - halfH;
        }

        float diff = targetY - paddleCenterY;

        float maxSpeed = aiPaddle->speed * speedMult;
        float desiredVy = diff * reactionMult;
        if (desiredVy > maxSpeed) desiredVy = maxSpeed;
        if (desiredVy < -maxSpeed) desiredVy = -maxSpeed;

        aiPaddle->setVerticalSpeed(desiredVy);

        // AI horizontal movement in free movement mode
        if (freeMovement) {
            float aiDefaultX = (playerSide == 1)
                ? (float)(width - 40 - aiPaddle->width)
                : 40.0f;
            float advanceTarget = aiDefaultX;
            if (ballMovingTowardAI && !shouldDodge) {
                float advanceFraction = 0.0f;
                switch (aiDifficulty) {
                    case AI_EASY:   advanceFraction = 0.05f; break;
                    case AI_MEDIUM: advanceFraction = 0.15f; break;
                    case AI_HARD:   advanceFraction = 0.25f; break;
                }
                // Health-aware: advance less when health is low
                if (gameMode == MODE_BATTLE && aiHealth <= 3) {
                    advanceFraction *= 0.3f;
                }
                float centerX = (float)width * 0.5f;
                advanceTarget = aiDefaultX + (centerX - aiDefaultX) * advanceFraction;
            }
            float hDiff = advanceTarget - (aiPaddle->x + aiPaddle->width * 0.5f);
            float hSpeed = hDiff * reactionMult;
            if (hSpeed > maxSpeed) hSpeed = maxSpeed;
            if (hSpeed < -maxSpeed) hSpeed = -maxSpeed;
            aiPaddle->setHorizontalSpeed(hSpeed);

            // AI shield pickup seeking (overrides normal tracking when seeking)
            if (shieldEnabled && gameMode == MODE_BATTLE && ball->isPiercing) {
                ShieldPickup& aiPickup = (playerSide == 1) ? shieldPickup2 : shieldPickup1;
                bool aiHasShield = (playerSide == 1) ? shieldHeld2 : shieldHeld1;
                if (aiPickup.active && !aiHasShield) {
                    bool seekPickup = false;
                    switch (aiDifficulty) {
                        case AI_EASY: seekPickup = false; break;
                        case AI_MEDIUM: {
                            float dist = std::fabs(aiPickup.y - (aiPaddle->y + aiPaddle->height * 0.5f));
                            seekPickup = (ballMovingTowardAI && dist < 150.0f);
                            break;
                        }
                        case AI_HARD: seekPickup = true; break;
                    }
                    if (seekPickup) {
                        float pickupCenterY = aiPickup.y;
                        float padCenterY = aiPaddle->y + aiPaddle->height * 0.5f;
                        float dy2 = pickupCenterY - padCenterY;
                        float seekVy = dy2 * 4.0f;
                        float maxSp = aiPaddle->speed * 0.8f;
                        if (seekVy > maxSp) seekVy = maxSp;
                        if (seekVy < -maxSp) seekVy = -maxSp;
                        aiPaddle->setVerticalSpeed(seekVy);

                        float pickupCenterX = aiPickup.x;
                        float padCenterX = aiPaddle->x + aiPaddle->width * 0.5f;
                        float dx2 = pickupCenterX - padCenterX;
                        float seekVx = dx2 * 4.0f;
                        if (seekVx > maxSp) seekVx = maxSp;
                        if (seekVx < -maxSp) seekVx = -maxSp;
                        aiPaddle->setHorizontalSpeed(seekVx);
                    }
                }
            }
        } else {
            aiPaddle->setHorizontalSpeed(0.0f);
        }
    } else {
        // PvP: each player can independently use keyboard or mouse
        float p1MinX = 0.0f;
        float p1MaxX = (float)width * 0.45f - (float)paddle1->width;
        float p2MinX = (float)width * 0.55f;
        float p2MaxX = (float)width - (float)paddle2->width;

        if (p1UseMouse) {
            float targetY = mouseY - paddle1->height * 0.5f;
            if (targetY < 0.0f) targetY = 0.0f;
            if (targetY > (float)(height - paddle1->height)) targetY = (float)(height - paddle1->height);
            paddle1->y = targetY;
            paddle1->setVerticalSpeed(0.0f);
            if (freeMovement) {
                float targetX = mouseX - paddle1->width * 0.5f;
                if (targetX < p1MinX) targetX = p1MinX;
                if (targetX > p1MaxX) targetX = p1MaxX;
                paddle1->x = targetX;
                paddle1->setHorizontalSpeed(0.0f);
            }
        } else {
            paddle1->handleInput(keys, freeMovement);
        }
        if (p2UseMouse) {
            float targetY = mouseY - paddle2->height * 0.5f;
            if (targetY < 0.0f) targetY = 0.0f;
            if (targetY > (float)(height - paddle2->height)) targetY = (float)(height - paddle2->height);
            paddle2->y = targetY;
            paddle2->setVerticalSpeed(0.0f);
            if (freeMovement) {
                float targetX = mouseX - paddle2->width * 0.5f;
                if (targetX < p2MinX) targetX = p2MinX;
                if (targetX > p2MaxX) targetX = p2MaxX;
                paddle2->x = targetX;
                paddle2->setHorizontalSpeed(0.0f);
            }
        } else {
            paddle2->handleInput(keys, freeMovement);
        }
    }

    {
        float p1MinX = 0.0f;
        float p1MaxX = (float)width * 0.45f - (float)paddle1->width;
        float p2MinX = (float)width * 0.55f;
        float p2MaxX = (float)width - (float)paddle2->width;
        paddle1->move(dt, height, freeMovement, p1MinX, p1MaxX);
        paddle2->move(dt, height, freeMovement, p2MinX, p2MaxX);
    }
    
    // Restore original speeds
    paddle1->speed = originalSpeed1;
    paddle2->speed = originalSpeed2;

    // Boost particle update & emission
    if (fxEnabled) {
        float fdt = (float)dt;
        // Update existing particles
        for (int i = 0; i < boostParticleCount1; ++i) {
            BoostParticle& p = boostParticles1[i];
            p.x += p.vx * fdt;
            p.y += p.vy * fdt;
            p.life -= fdt;
            if (p.life <= 0.0f) {
                boostParticles1[i] = boostParticles1[--boostParticleCount1];
                --i;
            }
        }
        for (int i = 0; i < boostParticleCount2; ++i) {
            BoostParticle& p = boostParticles2[i];
            p.x += p.vx * fdt;
            p.y += p.vy * fdt;
            p.life -= fdt;
            if (p.life <= 0.0f) {
                boostParticles2[i] = boostParticles2[--boostParticleCount2];
                --i;
            }
        }
        // Emit new particles when boosting
        float emitInterval = 0.03f;
        if (boostActive1) {
            boostEmitTimer1 += fdt;
            while (boostEmitTimer1 >= emitInterval && boostParticleCount1 < MaxBoostParticles) {
                boostEmitTimer1 -= emitInterval;
                BoostParticle& p = boostParticles1[boostParticleCount1++];
                float randY = ((std::rand() % 100) / 100.0f);
                p.x = paddle1->x;
                p.y = paddle1->y + randY * paddle1->height;
                p.vx = -40.0f - (std::rand() % 60);
                p.vy = ((std::rand() % 100) - 50) * 0.6f;
                p.life = 0.3f + (std::rand() % 30) * 0.01f;
                p.maxLife = p.life;
                p.size = 2.0f + (std::rand() % 30) * 0.1f;
                p.r = 0.3f; p.g = 0.7f; p.b = 1.0f;
                p.active = true;
            }
        } else {
            boostEmitTimer1 = 0.0f;
        }
        if (boostActive2) {
            boostEmitTimer2 += fdt;
            while (boostEmitTimer2 >= emitInterval && boostParticleCount2 < MaxBoostParticles) {
                boostEmitTimer2 -= emitInterval;
                BoostParticle& p = boostParticles2[boostParticleCount2++];
                float randY = ((std::rand() % 100) / 100.0f);
                p.x = paddle2->x + paddle2->width;
                p.y = paddle2->y + randY * paddle2->height;
                p.vx = 40.0f + (std::rand() % 60);
                p.vy = ((std::rand() % 100) - 50) * 0.6f;
                p.life = 0.3f + (std::rand() % 30) * 0.01f;
                p.maxLife = p.life;
                p.size = 2.0f + (std::rand() % 30) * 0.1f;
                p.r = 0.3f; p.g = 0.7f; p.b = 1.0f;
                p.active = true;
            }
        } else {
            boostEmitTimer2 = 0.0f;
        }
    }

    // Track shield state before ball update so we can detect consumption
    bool s1Before = shieldHeld1;
    bool s2Before = shieldHeld2;
    ball->update(dt, width, height, *paddle1, *paddle2, score1, score2, health1, health2, boostMeter1, boostMeter2, gameMode == MODE_BATTLE, shieldHeld1, shieldHeld2);
    // Start cooldown when shield is consumed (fixed mode only)
    if (!freeMovement && shieldEnabled && gameMode == MODE_BATTLE) {
        if (s1Before && !shieldHeld1) shieldCooldown1 = ShieldCooldown;
        if (s2Before && !shieldHeld2) shieldCooldown2 = ShieldCooldown;
    }
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
    // Check KO win condition (health depleted) - Battle Mode only
    if (gameMode == MODE_BATTLE && (health1 <= 0 || health2 <= 0)) {
        gameOver = true;
        paused = true;
        koWin = true;
        currentMenu = MENU_NONE;

        inWinLoseScreen = true;
        winLoseTimer = 0.0;
        winLoseShowPrompt = false;

        lastWinner = (health2 <= 0) ? 1 : 2;
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
                    s.vx = dirX * 320.0f;
                    s.vy = -100.0f + dirY * 100.0f;
                    s.ax = 0.0f;
                    s.ay = 750.0f;

                    s.size = 8.0f + ((float)std::rand() / (float)RAND_MAX) * 18.0f;
                    s.angle = ((float)std::rand() / (float)RAND_MAX) * 6.2831853f;
                    s.angularVelocity = -5.0f + ((float)std::rand() / (float)RAND_MAX) * 10.0f;
                    s.life = 0.0f;
                    s.maxLife = 2.0f + ((float)std::rand() / (float)RAND_MAX) * 0.8f;
                    s.r = losingPaddle->colorR;
                    s.g = losingPaddle->colorG;
                    s.b = losingPaddle->colorB;
                    s.active = true;
                }
            }

            loseWispCount = MaxLoseWisps;
            for (int i = 0; i < loseWispCount; ++i) {
                LoseWisp& w = loseWisps[i];
                float t = (float)i / (float)(loseWispCount > 0 ? loseWispCount : 1);

                float baseX = paddle1 ? paddle1->x + paddle1->width * 0.5f : (float)width * 0.25f;
                float baseY = paddle1 ? paddle1->y + paddle1->height * 0.5f : (float)height * 0.5f;

                w.x = baseX + (t - 0.5f) * 100.0f;
                w.y = baseY + 30.0f;
                w.vx = ((float)std::rand() / (float)RAND_MAX - 0.5f) * 60.0f;
                w.vy = -60.0f - ((float)std::rand() / (float)RAND_MAX) * 60.0f;
                w.life = 0.0f;
                w.maxLife = 2.5f + ((float)std::rand() / (float)RAND_MAX) * 1.5f;

                float cool = 0.5f + 0.5f * t;
                w.r = paddle1 ? paddle1->colorR * 0.5f * cool : 0.5f * cool;
                w.g = paddle1 ? paddle1->colorG * 0.7f * cool : 0.6f * cool;
                w.b = paddle1 ? paddle1->colorB : 1.0f;
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
    }

    // Check competitive win condition if enabled
    if (!endlessMode && targetScore > 0 && !gameOver) {
        if (score1 >= targetScore || score2 >= targetScore) {
            koWin = false;
            gameOver = true;
            paused = true;
            currentMenu = MENU_NONE;

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
        // Boost color tint: shift paddle toward cyan when boosting
        float savedR1 = 0, savedG1 = 0, savedB1 = 0;
        float savedR2 = 0, savedG2 = 0, savedB2 = 0;
        if (paddle1 && boostActive1 && fxEnabled) {
            savedR1 = paddle1->colorR; savedG1 = paddle1->colorG; savedB1 = paddle1->colorB;
            float pulse = 0.6f + 0.4f * std::sin((float)labelTimer * 10.0f);
            float blend = 0.4f * pulse;
            paddle1->colorR = savedR1 * (1.0f - blend) + 0.3f * blend;
            paddle1->colorG = savedG1 * (1.0f - blend) + 0.85f * blend;
            paddle1->colorB = savedB1 * (1.0f - blend) + 1.0f * blend;
        }
        if (paddle2 && boostActive2 && fxEnabled) {
            savedR2 = paddle2->colorR; savedG2 = paddle2->colorG; savedB2 = paddle2->colorB;
            float pulse = 0.6f + 0.4f * std::sin((float)labelTimer * 10.0f);
            float blend = 0.4f * pulse;
            paddle2->colorR = savedR2 * (1.0f - blend) + 0.3f * blend;
            paddle2->colorG = savedG2 * (1.0f - blend) + 0.85f * blend;
            paddle2->colorB = savedB2 * (1.0f - blend) + 1.0f * blend;
        }

        if (paddle1) paddle1->render();
        if (paddle2) paddle2->render();
        if (ball) ball->render();

        // Restore paddle colors
        if (paddle1 && boostActive1 && fxEnabled) {
            paddle1->colorR = savedR1; paddle1->colorG = savedG1; paddle1->colorB = savedB1;
        }
        if (paddle2 && boostActive2 && fxEnabled) {
            paddle2->colorR = savedR2; paddle2->colorG = savedG2; paddle2->colorB = savedB2;
        }
    }

    // Switch to 2D for HUD and menus on top
    glDisable(GL_DEPTH_TEST);
    resetProjection();
    glLoadIdentity();

    // Boost visual effects: paddle glow aura + particles
    if (fxEnabled) {
        // Paddle glow aura when boosting
        auto drawBoostGlow = [&](Paddle* pad, bool active) {
            if (!pad || !active) return;
            float pulse = 0.5f + 0.5f * std::sin((float)labelTimer * 8.0f);
            float glowPad = 12.0f + 4.0f * pulse;
            float gx0 = pad->x - glowPad;
            float gy0 = pad->y - glowPad;
            float gx1 = pad->x + pad->width + glowPad;
            float gy1 = pad->y + pad->height + glowPad;
            float cx = pad->x + pad->width * 0.5f;
            float cy = pad->y + pad->height * 0.5f;
            float alpha = 0.25f + 0.15f * pulse;

            // Radial glow using triangle fan
            glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.3f, 0.8f, 1.0f, alpha);
            glVertex2f(cx, cy);
            glColor4f(0.2f, 0.6f, 1.0f, 0.0f);
            glVertex2f(gx0, gy0);
            glVertex2f(gx1, gy0);
            glVertex2f(gx1, gy1);
            glVertex2f(gx0, gy1);
            glVertex2f(gx0, gy0);
            glEnd();
        };
        drawBoostGlow(paddle1, boostActive1);
        drawBoostGlow(paddle2, boostActive2);

        // Boost particles
        for (int i = 0; i < boostParticleCount1; ++i) {
            const BoostParticle& p = boostParticles1[i];
            float t = p.life / p.maxLife;
            float alpha = t * 0.8f;
            float sz = p.size * t;
            glBegin(GL_QUADS);
            glColor4f(p.r, p.g, p.b, alpha);
            glVertex2f(p.x - sz, p.y - sz);
            glVertex2f(p.x + sz, p.y - sz);
            glVertex2f(p.x + sz, p.y + sz);
            glVertex2f(p.x - sz, p.y + sz);
            glEnd();
        }
        for (int i = 0; i < boostParticleCount2; ++i) {
            const BoostParticle& p = boostParticles2[i];
            float t = p.life / p.maxLife;
            float alpha = t * 0.8f;
            float sz = p.size * t;
            glBegin(GL_QUADS);
            glColor4f(p.r, p.g, p.b, alpha);
            glVertex2f(p.x - sz, p.y - sz);
            glVertex2f(p.x + sz, p.y - sz);
            glVertex2f(p.x + sz, p.y + sz);
            glVertex2f(p.x - sz, p.y + sz);
            glEnd();
        }
    }

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
        float alpha = (currentMenu == MENU_MAIN || currentMenu == MENU_PLAY || currentMenu == MENU_SETTINGS) ? 1.0f : 0.5f;
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
    if (currentMenu != MENU_MAIN && currentMenu != MENU_PLAY) {
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
            const char* modePrefix = (gameMode == MODE_BATTLE) ? "BATTLE" : "CLASSIC";
            if (endlessMode) {
                std::snprintf(modeText, sizeof(modeText), "%s ENDLESS AI", modePrefix);
            } else if (targetScore > 0) {
                std::snprintf(modeText, sizeof(modeText), "%s FIRST TO %d AI", modePrefix, targetScore);
            } else {
                std::snprintf(modeText, sizeof(modeText), "%s VS AI", modePrefix);
            }
        } else {
            const char* modePrefix = (gameMode == MODE_BATTLE) ? "BATTLE" : "CLASSIC";
            if (targetScore > 0) {
                std::snprintf(modeText, sizeof(modeText), "%s FIRST TO %d PVP", modePrefix, targetScore);
            } else if (endlessMode) {
                std::snprintf(modeText, sizeof(modeText), "%s ENDLESS PVP", modePrefix);
            } else {
                std::snprintf(modeText, sizeof(modeText), "%s P1 VS P2", modePrefix);
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

        // Health bars near each paddle (Battle Mode only)
        if (fxEnabled && gameMode == MODE_BATTLE) {
            float barWidth = 80.0f;
            float barHeight = 10.0f;
            float barPadding = 2.0f;

            // P1 health bar (left side)
            float p1BarX = paddle1->x + paddle1->width + 15.0f;
            float p1BarY = paddle1->y + paddle1->height * 0.5f - barHeight * 0.5f;

            // Background
            glBegin(GL_QUADS);
            glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
            glVertex2f(p1BarX - barPadding, p1BarY - barPadding);
            glVertex2f(p1BarX + barWidth + barPadding, p1BarY - barPadding);
            glVertex2f(p1BarX + barWidth + barPadding, p1BarY + barHeight + barPadding);
            glVertex2f(p1BarX - barPadding, p1BarY + barHeight + barPadding);
            glEnd();

            // Health fill with color shift (green to yellow to red)
            float p1Ratio = (float)health1 / (float)maxHealth;
            float p1FillWidth = barWidth * p1Ratio;
            float p1Hue = p1Ratio * 0.33f; // 0.33 = green, 0 = red
            float p1r, p1g, p1b;
            hsvToRgb(p1Hue, 0.9f, 1.0f, p1r, p1g, p1b);

            // Animated pulse when low health
            float p1Pulse = 1.0f;
            if (health1 <= 2) {
                p1Pulse = 0.7f + 0.3f * std::sin((float)labelTimer * 8.0f);
            }

            glBegin(GL_QUADS);
            glColor4f(p1r * p1Pulse, p1g * p1Pulse, p1b * p1Pulse, 1.0f);
            glVertex2f(p1BarX, p1BarY);
            glVertex2f(p1BarX + p1FillWidth, p1BarY);
            glVertex2f(p1BarX + p1FillWidth, p1BarY + barHeight);
            glVertex2f(p1BarX, p1BarY + barHeight);
            glEnd();

            // P2 health bar (right side)
            float p2BarX = paddle2->x - 15.0f - barWidth;
            float p2BarY = paddle2->y + paddle2->height * 0.5f - barHeight * 0.5f;

            // Background
            glBegin(GL_QUADS);
            glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
            glVertex2f(p2BarX - barPadding, p2BarY - barPadding);
            glVertex2f(p2BarX + barWidth + barPadding, p2BarY - barPadding);
            glVertex2f(p2BarX + barWidth + barPadding, p2BarY + barHeight + barPadding);
            glVertex2f(p2BarX - barPadding, p2BarY + barHeight + barPadding);
            glEnd();

            // Health fill
            float p2Ratio = (float)health2 / (float)maxHealth;
            float p2FillWidth = barWidth * p2Ratio;
            float p2Hue = p2Ratio * 0.33f;
            float p2r, p2g, p2b;
            hsvToRgb(p2Hue, 0.9f, 1.0f, p2r, p2g, p2b);

            float p2Pulse = 1.0f;
            if (health2 <= 2) {
                p2Pulse = 0.7f + 0.3f * std::sin((float)labelTimer * 8.0f);
            }

            glBegin(GL_QUADS);
            glColor4f(p2r * p2Pulse, p2g * p2Pulse, p2b * p2Pulse, 1.0f);
            glVertex2f(p2BarX + barWidth - p2FillWidth, p2BarY);
            glVertex2f(p2BarX + barWidth, p2BarY);
            glVertex2f(p2BarX + barWidth, p2BarY + barHeight);
            glVertex2f(p2BarX + barWidth - p2FillWidth, p2BarY + barHeight);
            glEnd();
        }

        // Boost meters (shown in both Classic and Battle modes)
        if (fxEnabled) {
            float boostBarWidth = 60.0f;
            float boostBarHeight = 6.0f;
            // Position boost bar relative to paddle center; offset further down if health bars visible
            float boostYOffset = (gameMode == MODE_BATTLE) ? 18.0f : 8.0f;

            // P1 boost bar
            float b1X = paddle1->x + paddle1->width + 15.0f;
            float b1Y = paddle1->y + paddle1->height * 0.5f + boostYOffset;

            glBegin(GL_QUADS);
            glColor4f(0.15f, 0.15f, 0.2f, 0.7f);
            glVertex2f(b1X - 1, b1Y - 1);
            glVertex2f(b1X + boostBarWidth + 1, b1Y - 1);
            glVertex2f(b1X + boostBarWidth + 1, b1Y + boostBarHeight + 1);
            glVertex2f(b1X - 1, b1Y + boostBarHeight + 1);
            glEnd();

            float b1Fill = boostBarWidth * boostMeter1;
            float b1Glow = boostActive1 ? (0.8f + 0.2f * std::sin((float)labelTimer * 12.0f)) : 1.0f;
            glBegin(GL_QUADS);
            glColor4f(0.2f * b1Glow, 0.6f * b1Glow, 1.0f * b1Glow, 1.0f);
            glVertex2f(b1X, b1Y);
            glVertex2f(b1X + b1Fill, b1Y);
            glVertex2f(b1X + b1Fill, b1Y + boostBarHeight);
            glVertex2f(b1X, b1Y + boostBarHeight);
            glEnd();

            // P2 boost bar
            float b2X = paddle2->x - 15.0f - boostBarWidth;
            float b2Y = paddle2->y + paddle2->height * 0.5f + boostYOffset;

            glBegin(GL_QUADS);
            glColor4f(0.15f, 0.15f, 0.2f, 0.7f);
            glVertex2f(b2X - 1, b2Y - 1);
            glVertex2f(b2X + boostBarWidth + 1, b2Y - 1);
            glVertex2f(b2X + boostBarWidth + 1, b2Y + boostBarHeight + 1);
            glVertex2f(b2X - 1, b2Y + boostBarHeight + 1);
            glEnd();

            float b2Fill = boostBarWidth * boostMeter2;
            float b2Glow = boostActive2 ? (0.8f + 0.2f * std::sin((float)labelTimer * 12.0f)) : 1.0f;
            glBegin(GL_QUADS);
            glColor4f(0.2f * b2Glow, 0.6f * b2Glow, 1.0f * b2Glow, 1.0f);
            glVertex2f(b2X + boostBarWidth - b2Fill, b2Y);
            glVertex2f(b2X + boostBarWidth, b2Y);
            glVertex2f(b2X + boostBarWidth, b2Y + boostBarHeight);
            glVertex2f(b2X + boostBarWidth - b2Fill, b2Y + boostBarHeight);
            glEnd();
        }

        // Shield visual effects (Battle Mode only)
        if (shieldEnabled && gameMode == MODE_BATTLE && fxEnabled) {
            float pickupSize = 18.0f;

            // Draw collectible shield pickups on field (free movement mode)
            if (freeMovement) {
                auto drawPickup = [&](const ShieldPickup& pickup) {
                    if (!pickup.active) return;
                    float cx = pickup.x;
                    float cy = pickup.y + std::sin(pickup.bobTimer * 3.0f) * 5.0f;
                    float pulse = 0.7f + 0.3f * std::sin(pickup.bobTimer * 4.0f);
                    float sz = pickupSize * 0.5f;

                    // Shield shape: chevron/pentagon pointing up
                    // 5 points: top, upper-right, lower-right, bottom, lower-left
                    float pts[5][2] = {
                        { cx,          cy - sz },        // top
                        { cx + sz,     cy - sz * 0.3f }, // upper-right
                        { cx + sz * 0.6f, cy + sz },     // lower-right
                        { cx - sz * 0.6f, cy + sz },     // lower-left
                        { cx - sz,     cy - sz * 0.3f }  // upper-left
                    };

                    // Outer glow
                    float glowR = 12.0f;
                    glBegin(GL_TRIANGLE_FAN);
                    glColor4f(0.3f, 0.9f, 0.5f, 0.25f * pulse);
                    glVertex2f(cx, cy);
                    glColor4f(0.2f, 0.8f, 0.4f, 0.0f);
                    int glowSegs = 20;
                    for (int i = 0; i <= glowSegs; ++i) {
                        float ang = (float)i / (float)glowSegs * 6.2831853f;
                        glVertex2f(cx + std::cos(ang) * (sz + glowR), cy + std::sin(ang) * (sz + glowR));
                    }
                    glEnd();

                    // Filled shield shape
                    glBegin(GL_TRIANGLE_FAN);
                    glColor4f(0.3f, 1.0f, 0.6f, 0.8f * pulse);
                    glVertex2f(cx, cy);
                    for (int i = 0; i <= 4; ++i) {
                        int idx = i % 5;
                        glColor4f(0.2f, 0.85f, 0.5f, 0.7f * pulse);
                        glVertex2f(pts[idx][0], pts[idx][1]);
                    }
                    glEnd();

                    // Outline
                    glLineWidth(2.0f);
                    glBegin(GL_LINE_LOOP);
                    glColor4f(0.5f, 1.0f, 0.8f, 0.9f * pulse);
                    for (int i = 0; i < 5; ++i) {
                        glVertex2f(pts[i][0], pts[i][1]);
                    }
                    glEnd();
                };
                drawPickup(shieldPickup1);
                drawPickup(shieldPickup2);
            }

            // Shield indicator on paddle when holding a shield (both modes)
            auto drawShieldIndicator = [&](Paddle* pad, bool held) {
                if (!pad || !held) return;
                float pulse = 0.6f + 0.4f * std::sin((float)labelTimer * 6.0f);
                float alpha = 0.4f * pulse;
                bool isLeft = (pad->upKey == SDL_SCANCODE_W);
                float bx = isLeft ? (pad->x + pad->width + 2.0f) : (pad->x - 6.0f);
                float by0 = pad->y - 4.0f;
                float by1 = pad->y + pad->height + 4.0f;
                float bw = 4.0f;
                // Barrier quad on paddle face
                glBegin(GL_QUADS);
                glColor4f(0.4f, 1.0f, 0.6f, alpha);
                glVertex2f(bx, by0);
                glVertex2f(bx + bw, by0);
                glColor4f(0.2f, 0.9f, 1.0f, alpha * 0.7f);
                glVertex2f(bx + bw, by1);
                glVertex2f(bx, by1);
                glEnd();
                // Outer glow
                float glowW = 8.0f;
                glBegin(GL_QUADS);
                glColor4f(0.3f, 0.9f, 0.7f, alpha * 0.3f);
                glVertex2f(bx, by0);
                glVertex2f(bx + bw, by0);
                glColor4f(0.2f, 0.8f, 1.0f, 0.0f);
                if (isLeft) {
                    glVertex2f(bx + bw + glowW, by1);
                    glVertex2f(bx + bw + glowW, by0);
                } else {
                    glVertex2f(bx - glowW, by1);
                    glVertex2f(bx - glowW, by0);
                }
                glEnd();
            };
            drawShieldIndicator(paddle1, shieldHeld1);
            drawShieldIndicator(paddle2, shieldHeld2);

            // Fixed movement mode: cooldown bar below boost bar
            if (!freeMovement) {
                float shieldBarW = 40.0f;
                float shieldBarH = 4.0f;
                float shieldYOff = 30.0f;

                auto drawShieldCooldown = [&](Paddle* pad, float cooldown, bool held) {
                    if (!pad) return;
                    bool isLeft = (pad->upKey == SDL_SCANCODE_W);
                    float sx, sy;
                    if (isLeft) {
                        sx = pad->x + pad->width + 15.0f;
                    } else {
                        sx = pad->x - 15.0f - shieldBarW;
                    }
                    sy = pad->y + pad->height * 0.5f + shieldYOff;

                    // Background
                    glBegin(GL_QUADS);
                    glColor4f(0.1f, 0.15f, 0.1f, 0.6f);
                    glVertex2f(sx - 1, sy - 1);
                    glVertex2f(sx + shieldBarW + 1, sy - 1);
                    glVertex2f(sx + shieldBarW + 1, sy + shieldBarH + 1);
                    glVertex2f(sx - 1, sy + shieldBarH + 1);
                    glEnd();

                    float fill = 1.0f;
                    float r = 0.3f, g = 0.9f, b = 0.5f;
                    if (held) {
                        // Shield held: full bright green
                        fill = 1.0f;
                        r = 0.4f; g = 1.0f; b = 0.7f;
                    } else if (cooldown > 0.0f) {
                        // Cooling down
                        fill = 1.0f - (cooldown / ShieldCooldown);
                        r = 0.2f; g = 0.4f; b = 0.3f;
                    }
                    float fillW = shieldBarW * fill;
                    if (isLeft) {
                        glBegin(GL_QUADS);
                        glColor4f(r, g, b, 0.9f);
                        glVertex2f(sx, sy);
                        glVertex2f(sx + fillW, sy);
                        glVertex2f(sx + fillW, sy + shieldBarH);
                        glVertex2f(sx, sy + shieldBarH);
                        glEnd();
                    } else {
                        glBegin(GL_QUADS);
                        glColor4f(r, g, b, 0.9f);
                        glVertex2f(sx + shieldBarW - fillW, sy);
                        glVertex2f(sx + shieldBarW, sy);
                        glVertex2f(sx + shieldBarW, sy + shieldBarH);
                        glVertex2f(sx + shieldBarW - fillW, sy + shieldBarH);
                        glEnd();
                    }
                };
                drawShieldCooldown(paddle1, shieldCooldown1, shieldHeld1);
                drawShieldCooldown(paddle2, shieldCooldown2, shieldHeld2);
            }
        }

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
        } else if (currentMenu == MENU_MAIN || currentMenu == MENU_PLAY || currentMenu == MENU_SETTINGS) {
            // --- Shared menu rendering helper ---
            const char* headerText = nullptr;
            const char* menuItems[12];
            char dynamicLabels[12][48];
            int itemCount = 0;

            if (currentMenu == MENU_MAIN) {
                headerText = "PINGPONG";
                if (hasActiveGame) {
                    menuItems[0] = "PLAY";
                    menuItems[1] = "CONTINUE";
                    menuItems[2] = "SETTINGS";
                    menuItems[3] = "QUIT";
                    itemCount = 4;
                } else {
                    menuItems[0] = "PLAY";
                    menuItems[1] = "SETTINGS";
                    menuItems[2] = "QUIT";
                    itemCount = 3;
                }
            } else if (currentMenu == MENU_PLAY) {
                headerText = "SELECT MODE";
                menuItems[0] = "CLASSIC VS AI";
                menuItems[1] = "CLASSIC PVP";
                menuItems[2] = "BATTLE VS AI";
                menuItems[3] = "BATTLE PVP";
                menuItems[4] = "BACK";
                itemCount = 5;
            } else if (currentMenu == MENU_SETTINGS) {
                headerText = "SETTINGS";
                // Build dynamic labels for toggle items
                if (endlessMode) {
                    std::snprintf(dynamicLabels[0], sizeof(dynamicLabels[0]), "SCORE: ENDLESS");
                } else {
                    std::snprintf(dynamicLabels[0], sizeof(dynamicLabels[0]), "SCORE: FIRST TO %d", targetScore);
                }
                const char* diffNames[] = { "EASY", "MEDIUM", "HARD" };
                std::snprintf(dynamicLabels[1], sizeof(dynamicLabels[1]), "AI: %s", diffNames[aiDifficulty]);
                std::snprintf(dynamicLabels[2], sizeof(dynamicLabels[2]), "SIDE: %s", playerSide == 1 ? "LEFT" : "RIGHT");
                std::snprintf(dynamicLabels[3], sizeof(dynamicLabels[3]), "MOVEMENT: %s", freeMovement ? "FREE" : "FIXED");
                std::snprintf(dynamicLabels[4], sizeof(dynamicLabels[4]), "BOOST: %s", autoBoostEnabled ? "AUTO" : "MANUAL");
                std::snprintf(dynamicLabels[5], sizeof(dynamicLabels[5]), "SHIELD: %s", shieldEnabled ? "ON" : "OFF");
                std::snprintf(dynamicLabels[6], sizeof(dynamicLabels[6]), "HP: %d", maxHealth);
                std::snprintf(dynamicLabels[7], sizeof(dynamicLabels[7]), "VISUAL FX: %s", fxEnabled ? "ON" : "OFF");
                std::snprintf(dynamicLabels[8], sizeof(dynamicLabels[8]), "P1 INPUT: %s", p1UseMouse ? "MOUSE" : "KEYBOARD");
                std::snprintf(dynamicLabels[9], sizeof(dynamicLabels[9]), "P2 INPUT: %s", p2UseMouse ? "MOUSE" : "KEYBOARD");
                for (int i = 0; i < 10; ++i) menuItems[i] = dynamicLabels[i];
                menuItems[10] = "BACK";
                itemCount = 11;
            }

            // Header
            float headerScale = (currentMenu == MENU_MAIN) ? 32.0f : 24.0f;
            float headerW = measureText(headerScale, headerText);
            float hx = (float)width * 0.5f - headerW * 0.5f;
            float hy = (float)height * 0.25f;

            // Header shadow
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
            drawText(hx + 2.0f, hy + 2.0f, headerScale, headerText);

            // Header color - accent for title, white for submenus
            if (currentMenu == MENU_MAIN) {
                glColor4f(0.3f, 0.7f, 1.0f, 1.0f);
            } else {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            }
            drawText(hx, hy, headerScale, headerText);

            // Separator line under header
            float sepY = hy + headerScale * 1.3f;
            float sepW = 200.0f;
            glBegin(GL_QUADS);
            glColor4f(0.3f, 0.5f, 0.8f, 0.6f);
            glVertex2f((float)width * 0.5f - sepW * 0.5f, sepY);
            glVertex2f((float)width * 0.5f + sepW * 0.5f, sepY);
            glVertex2f((float)width * 0.5f + sepW * 0.5f, sepY + 2.0f);
            glVertex2f((float)width * 0.5f - sepW * 0.5f, sepY + 2.0f);
            glEnd();

            // Menu items with visual hierarchy
            float menuScale = 18.0f;
            float itemSpacing = menuScale * 1.6f;
            float startY = sepY + 20.0f;

            for (int i = 0; i < itemCount; ++i) {
                const char* label = menuItems[i];
                float w = measureText(menuScale, label);
                float ix = (float)width * 0.5f - w * 0.5f;
                float iy = startY + i * itemSpacing;

                bool isSelected = (i == menuSelection);
                bool isBack = (currentMenu == MENU_PLAY && i == 4) ||
                              (currentMenu == MENU_SETTINGS && i == 10);
                bool isQuit = (currentMenu == MENU_MAIN && i == itemCount - 1);

                if (isSelected) {
                    // Selection glow background
                    float glowPulse = 0.6f + 0.4f * std::sin((float)menuAnimTimer * 5.0f);
                    float bgPad = 8.0f;
                    glBegin(GL_QUADS);
                    glColor4f(0.2f, 0.4f, 0.8f, 0.25f * glowPulse);
                    glVertex2f(ix - bgPad, iy - 2.0f);
                    glVertex2f(ix + w + bgPad, iy - 2.0f);
                    glVertex2f(ix + w + bgPad, iy + menuScale + 2.0f);
                    glVertex2f(ix - bgPad, iy + menuScale + 2.0f);
                    glEnd();

                    // Selection indicator brackets
                    float bracketOffset = 12.0f;
                    glColor4f(0.3f, 0.7f, 1.0f, 1.0f);
                    drawText(ix - bracketOffset - measureText(menuScale, ">"), iy, menuScale, ">");
                    drawText(ix + w + bracketOffset, iy, menuScale, "<");

                    // Highlighted text color
                    if (isQuit) {
                        glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
                    } else {
                        glColor4f(1.0f, 1.0f, 0.3f, 1.0f);
                    }
                } else {
                    if (isBack) {
                        glColor4f(0.6f, 0.6f, 0.7f, 1.0f);
                    } else if (isQuit) {
                        glColor4f(0.7f, 0.4f, 0.4f, 1.0f);
                    } else if (currentMenu == MENU_MAIN && i == 0) {
                        // PLAY is visually prominent
                        glColor4f(0.9f, 0.9f, 1.0f, 1.0f);
                    } else {
                        glColor4f(0.8f, 0.8f, 0.85f, 1.0f);
                    }
                }

                drawText(ix, iy, menuScale, label);
            }

            // Navigation hint at bottom
            const char* hint = "UP/DOWN: NAVIGATE   ENTER/CLICK: SELECT   ESC: BACK";
            float hintScale = 10.0f;
            float hintW = measureText(hintScale, hint);
            float hintX = (float)width * 0.5f - hintW * 0.5f;
            float hintY = (float)height - 40.0f;
            glColor4f(0.5f, 0.5f, 0.6f, 0.7f);
            drawText(hintX, hintY, hintScale, hint);
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

                // Radial glow backdrop (rendered before animations)
                if (fxEnabled) {
                    float t = (float)winLoseTimer;
                    float pulse = 0.7f + 0.3f * std::sin(t * 3.5f);
                    float radius = (float)width * 0.25f + pulse * 30.0f;
                    float alpha = 0.4f;

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

                // Determine title text
                const char* title = "GAME OVER";
                const char* subtitle = nullptr;
                if (koWin) {
                    title = "K.O.!";
                    if (singlePlayer) {
                        subtitle = lastAiWin ? "YOU LOSE" : "YOU WIN";
                    } else {
                        subtitle = (lastWinner == 1) ? "PLAYER 1 WINS" : "PLAYER 2 WINS";
                    }
                } else if (singlePlayer) {
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

                float baseScale = koWin ? 40.0f : (lastAiWin ? 26.0f : 30.0f);
                float pulse = 1.0f + (koWin ? 0.15f : 0.08f) * std::sin((float)winLoseTimer * (koWin ? 6.0f : 4.0f));
                float titleScale = baseScale * pulse;

                float titleWidth = measureText(titleScale, title);
                float tx = centerX - titleWidth * 0.5f;
                float ty = centerY - titleScale * (koWin ? 1.8f : 1.2f);

                // Drop shadow for pseudo-3D depth
                float shadowOffset = koWin ? 4.0f : 3.0f;
                glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
                drawText(tx + shadowOffset, ty + shadowOffset, titleScale, title);

                // Main title color - KO is dramatic red/orange
                if (koWin) {
                    float koFlash = 0.7f + 0.3f * std::sin((float)winLoseTimer * 10.0f);
                    glColor4f(1.0f * koFlash, 0.3f * koFlash, 0.1f, 1.0f);
                } else if (lastAiWin) {
                    glColor4f(accentR * 1.4f, accentG * 0.3f, accentB * 0.4f, 1.0f);
                } else {
                    glColor4f(accentR * 1.4f, accentG * 1.4f, accentB * 1.4f, 1.0f);
                }
                drawText(tx, ty, titleScale, title);

                // KO subtitle (winner info)
                if (koWin && subtitle) {
                    float subScale = 20.0f;
                    float subWidth = measureText(subScale, subtitle);
                    float subX = centerX - subWidth * 0.5f;
                    float subY = ty + titleScale * 0.9f;
                    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
                    drawText(subX + 2.0f, subY + 2.0f, subScale, subtitle);
                    if (lastAiWin) {
                        glColor4f(0.9f, 0.4f, 0.4f, 1.0f);
                    } else {
                        glColor4f(0.4f, 1.0f, 0.5f, 1.0f);
                    }
                    drawText(subX, subY, subScale, subtitle);
                }

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
            } else if (currentMenu == MENU_PAUSE) {
                // In-game pause menu: clean 3-item layout
                const char* headerText = "PAUSED";
                float headerScale = 28.0f;
                float headerW = measureText(headerScale, headerText);
                float hx = (float)width * 0.5f - headerW * 0.5f;
                float hy = (float)height * 0.3f;

                glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
                drawText(hx + 2.0f, hy + 2.0f, headerScale, headerText);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                drawText(hx, hy, headerScale, headerText);

                // Separator
                float sepY = hy + headerScale * 1.3f;
                float sepW = 160.0f;
                glBegin(GL_QUADS);
                glColor4f(0.3f, 0.5f, 0.8f, 0.5f);
                glVertex2f((float)width * 0.5f - sepW * 0.5f, sepY);
                glVertex2f((float)width * 0.5f + sepW * 0.5f, sepY);
                glVertex2f((float)width * 0.5f + sepW * 0.5f, sepY + 2.0f);
                glVertex2f((float)width * 0.5f - sepW * 0.5f, sepY + 2.0f);
                glEnd();

                const char* pauseItems[3] = { "RESUME", "SETTINGS", "MAIN MENU" };
                float menuScale = 20.0f;
                float itemSpacing = menuScale * 1.8f;
                float startY = sepY + 24.0f;

                for (int i = 0; i < 3; ++i) {
                    const char* label = pauseItems[i];
                    float w = measureText(menuScale, label);
                    float ix = (float)width * 0.5f - w * 0.5f;
                    float iy = startY + i * itemSpacing;

                    if (i == menuSelection) {
                        float glowPulse = 0.6f + 0.4f * std::sin((float)menuAnimTimer * 5.0f);
                        float bgPad = 8.0f;
                        glBegin(GL_QUADS);
                        glColor4f(0.2f, 0.4f, 0.8f, 0.25f * glowPulse);
                        glVertex2f(ix - bgPad, iy - 2.0f);
                        glVertex2f(ix + w + bgPad, iy - 2.0f);
                        glVertex2f(ix + w + bgPad, iy + menuScale + 2.0f);
                        glVertex2f(ix - bgPad, iy + menuScale + 2.0f);
                        glEnd();

                        float bracketOffset = 12.0f;
                        glColor4f(0.3f, 0.7f, 1.0f, 1.0f);
                        drawText(ix - bracketOffset - measureText(menuScale, ">"), iy, menuScale, ">");
                        drawText(ix + w + bracketOffset, iy, menuScale, "<");

                        glColor4f(1.0f, 1.0f, 0.3f, 1.0f);
                    } else {
                        glColor4f(0.8f, 0.8f, 0.85f, 1.0f);
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
