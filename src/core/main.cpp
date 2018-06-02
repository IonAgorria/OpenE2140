#include "assets/assetimage.h"
#include "core/config.h"
#include "core/utils.h"
#include "core/io/log.h"
#include "core/graphics/window.h"
#include "assets/manager.h"
#include "SDL.h"
#include "SDL_keycode.h"
#include "luavm.h"

/**
 * Temporary event handler for asset viewing thing
 */
class ViewerEventHandler : public EventHandler {
    /** Viewing asset index */
    int index = 0;
    /** Viewing asset scale */
    int scale = 2;

public:
    /** Generated image of asset */
    std::unique_ptr<Image> image;
    /** Rectangle for drawing */
    Rectangle rectangle;
    /** Manager for asset fetching */
    std::shared_ptr<Manager> manager;
    /** Window for texture */
    std::shared_ptr<Window> window;

    void init(std::shared_ptr<Manager>& manager, std::shared_ptr<Window>& window) {
        this->manager = manager;
        this->window = window;
        loadImage();
    }

    void reset() {
        manager.reset();
        window.reset();
        image.reset();
    }

    void mouseMove(int x, int y) override {
        rectangle.setCenter(x, y);
    }

    void mouseClick(int x, int y, int button, bool press) override {
        if (!press) {
            switch (button) {
                case 1: //Left
                    if (1 < scale) {
                        scale--;
                    }
                    break;
                case 2: //Middle
                    scale = 1;
                    break;
                case 3: //Right
                    if (scale < 10) {
                        scale++;
                    }
                    break;
                default:
                    break;
            }
        }
        rectangle.setCenter(x, y);
        loadImage();
    }

    void keyChange(int code, std::string name, bool press) override {
        if (!press) {
            switch (code) {
                case SDLK_LEFT:
                    index -= 1;
                    break;
                case SDLK_RIGHT:
                    index += 1;
                    break;
                case SDLK_UP:
                    index += 50;
                    break;
                case SDLK_DOWN:
                    index -= 50;
                    break;
                default:
                    break;
            }
            log->debug("Key name: " + name);
            loadImage();
        }
    }

    void loadImage() {
        image.reset();
        std::shared_ptr<AssetImage> assetImage;
        if (index < 0) {
            assetImage = manager->getAsset<AssetImage>("MIX/SPRT3/" + std::to_string(-1 * index - 1));
        } else {
            assetImage = manager->getAsset<AssetImage>("MIX/SPRU0/" + std::to_string(index));
        }
        if (assetImage) {
            Vector2 imageSize = assetImage->getImageSize();
            std::unique_ptr<Image> newImage = std::make_unique<Image>(window->createTexture(imageSize), imageSize);
            if (newImage) {
                if (!assetImage->writeImage(*newImage)) {
                    log->error("{0}", assetImage->getError());
                }
                image = std::move(newImage);

                //Adjust rectangle
                Vector2 center;
                rectangle.getCenter(center);
                rectangle.w = imageSize.x * scale;
                rectangle.h = imageSize.y * scale;
                rectangle.setCenter(center);
            }
        }
        log->debug("Loaded index {0} image {1}", index, assetImage ? assetImage->toString() : "none");
    }
};

/**
 * Runs the interesting stuff
 *
 * @return if error occurred
 */
bool run() {
    auto lua = LuaVM::create();

    //Initialize event handler
    ViewerEventHandler viewer;

    //Initialize window
    std::shared_ptr<Window> window = std::make_shared<Window>(viewer);
    if (!window->init(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE)) {
        return true;
    }

    //Initialize asset manager
    std::shared_ptr<Manager> manager = std::make_shared<Manager>();
    if (!manager->init()) {
        return true;
    }

    //Configure viewer
    viewer.init(manager, window);

    //Main loop
    bool error = false;
    while (!window->isClosing()) {
        //Show viewer image
        if (viewer.image) {
            Image& image = *viewer.image;
            if (image && !window->draw(image, viewer.rectangle)) {
                error = true;
                break;
            }
        }

        //Update window
        if (!window->update()) {
            error = true;
            break;
        }
    }

    //Clean up
    viewer.reset();

    return error;
}

/**
 * Main program entry point
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
        error = run();
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
