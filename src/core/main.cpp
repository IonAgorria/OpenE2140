#include "assets/assetimage.h"
#include "core/common.h"
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

    //Parse args
    for(int i=1; i < argc; i++) {
        std::string arg = argv[i];
        std::transform(BEGIN_END(arg), arg.begin(), ::tolower);
        if (arg.compare("-debug") == 0 || arg.compare("-d") == 0) {
            Utils::setDebug(true);
        } else {
            std::cout << "Unknown arg " << arg << "\n";
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
        std::shared_ptr<Game> game = std::make_shared<Game>();
        error = !game->run();
        game.reset();
        if (error) {
            log->error("Error occurred");
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
