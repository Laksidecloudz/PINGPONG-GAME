#include "Game.h"
#include "Paddle.h"
#include "Ball.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <cstdio>
#include <cmath>

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
        segment(0.0f, 1.0f, 1.0f, 1.0f);
        segment(0.5f, 1.0f, 0.5f, 0.0f);
        break;
    case 'R':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(0.0f, 1.0f, 0.8f, 1.0f);
        segment(0.0f, 0.5f, 0.7f, 0.5f);
        segment(0.8f, 1.0f, 0.8f, 0.5f);
        segment(0.0f, 0.5f, 0.8f, 0.0f);
        break;
    case 'M':
        segment(0.0f, 0.0f, 0.0f, 1.0f);
        segment(1.0f, 0.0f, 1.0f, 1.0f);
        segment(0.0f, 1.0f, 0.5f, 0.5f);
        segment(1.0f, 1.0f, 0.5f, 0.5f);
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

Game::Game(int w, int h) : width(w), height(h), labelTimer(0.0), paused(false), pauseSelection(0) {}

bool Game::init() {
    Uint32 flags = SDL_INIT_VIDEO;
    bool ok = SDL_Init(flags);
    if (!ok) {
        const char* err = SDL_GetError();
        std::fprintf(stderr, "SDL_Init failed err='%s'\n", err ? err : "<null>");
        return false;
    }

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
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    resetProjection();

    // Create entities
    const int paddleW = 16;
    const int paddleH = 120;
    const float paddleSpeed = 600.0f;

    paddle1 = new Paddle(40.0f, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed, SDL_SCANCODE_W, SDL_SCANCODE_S);
    paddle2 = new Paddle(width - 40.0f - paddleW, (height - paddleH) * 0.5f, paddleW, paddleH, paddleSpeed, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

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

            if (sc == SDL_SCANCODE_ESCAPE) {
                paused = !paused;
                if (paused) {
                    pauseSelection = 0;
                }
                labelTimer = 0.0;
            } else if (paused) {
                const int menuItems = 4;
                if (sc == SDL_SCANCODE_UP) {
                    pauseSelection = (pauseSelection - 1 + menuItems) % menuItems;
                } else if (sc == SDL_SCANCODE_DOWN) {
                    pauseSelection = (pauseSelection + 1) % menuItems;
                } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
                    if (pauseSelection == 0) {
                        paused = false;
                    } else if (pauseSelection == 1) {
                        singlePlayer = true;
                        paused = false;
                    } else if (pauseSelection == 2) {
                        singlePlayer = false;
                        paused = false;
                    } else if (pauseSelection == 3) {
                        isRunning = false;
                    }
                    labelTimer = 0.0;
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

void Game::update(double dt) {
    if (paused) {
        return;
    }

    labelTimer += dt;

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
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Middle dashed line (optional visual)
    glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
    for (int y = 0; y < height; y += 30) {
        glBegin(GL_QUADS);
        float mx = width * 0.5f - 2.0f;
        glVertex2f(mx, (float)y);
        glVertex2f(mx + 4.0f, (float)y);
        glVertex2f(mx + 4.0f, (float)(y + 20));
        glVertex2f(mx, (float)(y + 20));
        glEnd();
    }

    // Entities
    paddle1->render();
    paddle2->render();
    ball->render();

    if (paused) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f((float)width, 0.0f);
        glVertex2f((float)width, (float)height);
        glVertex2f(0.0f, (float)height);
        glEnd();
    }

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    float topY = 30.0f;
    drawText(40.0f, topY, 12.0f, "P1: W/S");
    drawText((float)width - 200.0f, topY, 12.0f, "P2: UP/DOWN");

    // Scores centered at top
    char scoreText[32];
    std::snprintf(scoreText, sizeof(scoreText), "%d : %d", score1, score2);
    float scoreScale = 20.0f;
    float scoreWidth = measureText(scoreScale, scoreText);
    float scoreX = (float)width * 0.5f - scoreWidth * 0.5f;
    float scoreY = topY + 30.0f;
    drawText(scoreX, scoreY, scoreScale, scoreText);

    if (labelTimer < 3.0) {
        float labelScale = 14.0f;
        float labelYOffset = 30.0f;

        float p1Width = measureText(labelScale, "P1");
        float p2Width = measureText(labelScale, "P2");

        float p1X = paddle1->x + paddle1->width * 0.5f - p1Width * 0.5f;
        float p2X = paddle2->x + paddle2->width * 0.5f - p2Width * 0.5f;

        drawText(p1X, paddle1->y - labelYOffset, labelScale, "P1");
        drawText(p2X, paddle2->y - labelYOffset, labelScale, "P2");
    }

    if (paused) {
        const char* pausedText = "PAUSED";
        float pausedScale = 24.0f;
        float textWidth = measureText(pausedScale, pausedText);
        float x = (float)width * 0.5f - textWidth * 0.5f;
        float y = (float)height * 0.5f - pausedScale * 0.5f - 40.0f;
        drawText(x, y, pausedScale, pausedText);

        const char* items[4] = {
            "RESUME",
            "P1 VS AI",
            "P1 VS P2",
            "QUIT"
        };

        float menuScale = 18.0f;
        float startY = y + pausedScale * 1.8f;

        for (int i = 0; i < 4; ++i) {
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
    }

    // TODO: draw scores with a text system (placeholder)
    // This skeleton omits text rendering. We'll add it later.
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