#include <assets/assetimage.h>
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
                //TODO for testing, remove
                int index = 0;
                std::unique_ptr<Image> image;
                Rectangle rectangle;
                //Main loop
                SDL_Event event;
                bool quit = false;
                while (!quit && !error)
                {
                    //Handle any events
                    while (SDL_PollEvent(&event) == 1) {
                        switch (event.type) {
                            case SDL_MOUSEBUTTONDOWN:
                            case SDL_MOUSEBUTTONUP: {
                                log->debug("Mouse button: {0}", event.button.button);
                                break;
                            }
                            case SDL_MOUSEMOTION: {
                                rectangle.x = event.motion.x;
                                rectangle.y = event.motion.y;
                                //log->debug("Mouse motion: {0}x{1}", event.motion.x, event.motion.y);
                                break;
                            }
                            case SDL_KEYDOWN: {
                                break;
                            }
                            case SDL_KEYUP: {
                                int scancode = event.key.keysym.scancode;
                                log->debug("Key: {0}", scancode);
                                //TODO for testing, remove
                                switch (scancode) {
                                    case SDL_SCANCODE_LEFT:
                                        index -= 1;
                                        break;
                                    case SDL_SCANCODE_RIGHT:
                                        index += 1;
                                        break;
                                    case SDL_SCANCODE_UP:
                                        index += 50;
                                        break;
                                    case SDL_SCANCODE_DOWN:
                                        index -= 50;
                                        break;
                                    default:
                                        break;
                                }
                                image.reset();
                                std::shared_ptr<AssetImage> assetImage;
                                if (index < 0) {
                                    assetImage = manager.getAsset<AssetImage>("MIX/SPRT3/" + std::to_string(-1 * index - 1));
                                } else {
                                    assetImage = manager.getAsset<AssetImage>("MIX/SPRU0/" + std::to_string(index));
                                }
                                if (assetImage) {
                                    Vector2 imageSize = assetImage->getImageSize();
                                    std::unique_ptr<Image> newImage = std::make_unique<Image>(window.createTexture(imageSize), imageSize);
                                    if (newImage) {
                                        if (!assetImage->writeImage(*newImage)) {
                                            log->error("{0}", assetImage->getError());
                                        }
                                        image = std::move(newImage);
                                        rectangle.w = imageSize.x * 5;
                                        rectangle.h = imageSize.y * 5;
                                    }
                                }
                                log->debug("Loaded index {0} image {1}", index, assetImage ? assetImage->toString() : "none");
                                break;
                            }
                            case SDL_WINDOWEVENT: {
                                switch (event.window.event) {
                                    case SDL_WINDOWEVENT_RESIZED:
                                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                                        window.resize(event.window.data1, event.window.data2);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            }
                            case SDL_QUIT: {
                                quit = true;
                                continue;
                            }
                            default: {
                                continue;
                            }
                        }
                    }

                    //TODO for testing, remove
                    if (image) {
                        if (!window.draw(*image, rectangle)) {
                            error = true;
                            break;
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