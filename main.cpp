#include "Game.h"
#include "SDL3/SDL_main.h"

int main(int argc, char** argv) {
    Game game(1280, 720);
    if (!game.init()) {
        return 1;
    }
    game.run();
    game.clean();
    return 0;
}