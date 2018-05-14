#include "core/config.h"
#include "core/utils.h"
#include "core/io/log.h"
#include "core/graphics/window.h"
#include "core/math/rectangle.h"
#include "assets/manager.h"
#include "SDL.h"

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

    //Initialize SDL2
    bool error = false;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        Utils::showErrorDialog("SDL_Init failed\n" + Utils::checkSDLError(), log, false, true);
        error = true;
    } else {
        //Initialize window
        Window window(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE);
        if (!window) {
            error = true;
        } else {
            //Initialize manager
            Manager manager;
            if (!manager.initManager()) {
                error = true;
            } else {
                //Test;
                log->info("DIR {0}", (bool) manager.getAsset("PIRO/GRAPH/SHCKV00.PAL"));;
                log->info("WD {0}", (bool) manager.getAsset("MIX/GRAPH/DATAB.MIX"));
                log->info("MISSING {0}", (bool) manager.getAsset("LEVEL/NEW"));
                Rectangle rectangle(0, 0, 10, 10);
                Rectangle rectangleDst(0, 0, 200, 200);
                Image image(window.createTexture(rectangle.w, rectangle.h), rectangle);
                auto buffer = Utils::createBuffer(static_cast<const size_t>(rectangle.w * rectangle.h * 4));
                for (int i = 0; i < rectangle.w * rectangle.h * 4;) {
                    buffer[i++] = 0xFF;
                    buffer[i++] = byte (((i % 3) == 0) ? 0xFF : 0x00);
                    buffer[i++] = byte (((i % 3) == 0) ? 0xFF : 0x00);
                    buffer[i++] = byte (((i % 3) == 0) ? 0xFF : 0x00);
                }
                image.loadFromRGBA8888(log, buffer.get());

                //Main loop
                SDL_Event event;
                bool quit = false;
                while (!quit && !error)
                {
                    //Handle any events
                    while (SDL_PollEvent(&event) == 1) {
                        switch (event.type) {
                            case SDL_MOUSEBUTTONDOWN:
                            case SDL_MOUSEBUTTONUP:
                                log->debug("Mouse button: {0}", event.button.button);
                                break;
                            case SDL_MOUSEMOTION:
                                rectangleDst.x = event.motion.x;
                                rectangleDst.y = event.motion.y;
                                //log->debug("Mouse motion: {0}x{1}", event.motion.x, event.motion.y);
                                break;
                            case SDL_KEYDOWN:
                            case SDL_KEYUP:
                                log->debug("Key: {0}", event.key.keysym.scancode);
                                break;
                            case SDL_WINDOWEVENT:
                                switch (event.window.event) {
                                    case SDL_WINDOWEVENT_RESIZED:
                                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                                        window.resize(event.window.data1, event.window.data2);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case SDL_QUIT:
                                quit = true;
                            default:
                                continue;
                        }
                    }

                    //Show the screen
                    window.draw(image, rectangleDst);
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