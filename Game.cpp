#include "Game.h"
#include "Paddle.h"
#include "Ball.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <cstdio>
#include <cmath>

Game::Game(int w, int h) : width(w), height(h) {}

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
            if (e.key.scancode == SDL_SCANCODE_ESCAPE) {
                isRunning = false;
            }
        }
    }
}

void Game::update(double dt) {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    paddle1->handleInput(keys);
    paddle2->handleInput(keys);

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