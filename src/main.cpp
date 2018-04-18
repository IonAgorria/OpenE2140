#include "core/config.h"
#include "SDL.h"
#include "assets/manager.h"
#include "core/log.h"
#include "core/window.h"
#include "core/utils.h"

/**
 * Main program entry point
 *
 * @param argc number of args
 * @param argv args array
 * @return program exit code
 */
int main(int argc, char** argv) {
    //Register signal handler
    Utils::setSignalHandler(Utils::handleHaltAndCatchFire);

    //Enable args
    for(int i=1; i < argc; i++) {
        if (strcmp(argv[i], "Debug") == 0) {
            Utils::setDebug(true);
        }
    }

    //Initialize log
    log_ptr log = Log::get(MAIN_LOG);

    //Initialize SDL2
    bool error = false;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        Utils::showErrorDialog("SDL_Init failed: " + Utils::checkSDLError(), log, false);
        error = true;
    } else {
        //Initialize window
        Window window;
        if (!window.create(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE)) {
            error = true;
        } else {
            //Initialize manager
            Manager manager;
            if (!manager.loadContainers()) {
                error = true;
            } else {
                //Test
                auto asset = manager.getAsset("PIRO/GRAPH/SHCKV00.PAL");
                log->info("{0}", asset ? 1 : 0);
                asset = manager.getAsset("LEVEL/NEW");
                log->info("{0}", asset ? 1 : 0);

                //Main loop
                SDL_Event event{};
                Uint8 quit = 0;
                while (quit == 0 && !error)
                {
                    //Handle any events
                    while (SDL_PollEvent(&event) == 1) {
                        switch (event.type) {
                            case SDL_QUIT:
                                quit = 1;
                            case SDL_MOUSEBUTTONDOWN:
                            case SDL_MOUSEBUTTONUP:
                                log->debug("Mouse: {0}", event.button.button);
                                break;
                            case SDL_KEYDOWN:
                            case SDL_KEYUP:
                                log->debug("Key: {0}", event.key.keysym.scancode);
                                break;
                            default:
                                continue;
                        }
                    }

                    //Show the screen
                    if (!window.update()) {
                        error = true;
                        continue;
                    }
                }
            }
        }
    }

    //Close SDL (doesn't matter if SDL was init successfully)
    SDL_Quit();
    Utils::checkSDLError(log); //Show any error but don't stop now

    //Close the logs
    Log::closeAll();

    //Restore original sighandler
    Utils::restoreSignalHandler();

    //Return code
    return error ? 1 : 0;
}