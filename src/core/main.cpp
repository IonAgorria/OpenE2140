#include "assets/assetimage.h"
#include "core/config.h"
#include "core/utils.h"
#include "io/log.h"
#include "graphics/window.h"
#include "assets/manager.h"
#include "SDL.h"
#include "SDL_keycode.h"
#include "luavm.h"
#include "game.h"

/**
 * Main program entry point, does the basic initializations
 *
 * @param argc number of args
 * @param argv args array
 * @return program exit code
 */
int main(int argc, char** argv) {
    //Register signal and terminate handler
    Utils::setSignalHandler(Utils::handleHaltAndCatchFire, Utils::handleTerminate);

    //Cache the paths
    Utils::getInstallPath();
    Utils::getUserPath();

    //Enable args
    for(int i=1; i < argc; i++) {
        if (strcmp(argv[i], "Debug") == 0) {
            Utils::setDebug(true);
        }
    }

    //Initialize log
    log_ptr log = Log::get(MAIN_LOG);

    //Initialize SDL2 and run if success
    bool error;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        error = true;
        Utils::showErrorDialog("SDL_Init failed\n" + Utils::checkSDLError(), log, false, true);
    } else {
        Game game;
        error = !game.run();
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
