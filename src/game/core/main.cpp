#include "game.h"

/**
 * Main program entry point, does the basic initializations
 *
 * @param argc number of args
 * @param argv args array
 * @return program exit code
 */
int main(int argc, char** argv) {
    return Game::main(argc, argv, std::make_shared<Game>());
}
