//
// Created by Ion Agorria on 22/03/18
//
#include <cstdio>
#include <csignal>
#include <list>
#include <memory>
#include "common.h"
#include "SDL_filesystem.h"
#include "SDL_quit.h"
#include "SDL_messagebox.h"
#include "utils.h"

//Attempt to load boost libs
//#define GAME_USE_BOOST 0
#if GAME_USE_BOOST
#   define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED 1
#   include "boost/stacktrace.hpp"
#   include "boost/filesystem.hpp"
#endif

//Default static variable values
unsigned int Utils::flags = 0;
std::unique_ptr<std::string> Utils::installPath; //Default null pointer
std::unique_ptr<std::string> Utils::userPath; //Default null pointer
std::unique_ptr<std::string> Utils::dumpPath; //Default null pointer
std::terminate_handler Utils::originalTerminateHandler = nullptr;
std::string Utils::lastException;

void Utils::setFlag(unsigned int flag, bool value) {
    if (value) {
        BIT_ON(flags, flag);
    } else {
        BIT_OFF(flags, flag);
    }
}

bool Utils::isFlag(unsigned int flag) {
    return BIT_STATE(flags, flag);
}

std::string Utils::checkSDLError(const log_ptr& log) {
    const char* error = SDL_GetError();
    if (error && strlen(error) != 0) {
        SDL_ClearError();
        if (log) {
            log->error("SDL Error: {0}", error);
        }
        return std::string(error);
    }
    return "";
}

std::string Utils::checkGLError(const log_ptr& log) {
    std::string errors;
    GLenum result;
    int limit = 10;
    while((result = glGetError()) != GL_NO_ERROR && 0 < limit) {
        if (log) {
            log->error("GL Error: {0:x}", result);
        }
        if (errors.empty()) errors += "GL Error:";
        errors += " " + std::to_string(result);
    }
    return errors;
}

std::string Utils::checkAnyError(const log_ptr& log) {
    std::string result = checkSDLError(log);
    std::string glError = checkGLError(log);
    if (!result.empty() && !glError.empty()) {
        result += "\n";
    }
    result += glError;
    return result;
}

bool Utils::startsWith(const std::string& string, const std::string& start) {
    size_t startSize = start.size();
    if (startSize == 0) return true;
    if (string.size() < startSize) return false;
    return string.find(start, 0) == 0;
}

bool Utils::endsWith(const std::string& string, const std::string& end) {
    size_t endSize = end.size();
    if (endSize == 0) return true;
    size_t startPos = string.size() - endSize;
    if (startPos < 0) return false;
    return string.find(end, startPos) == startPos;
}

void Utils::setSignalHandler(sighandler signalHandler, std::terminate_handler terminateHandler) {
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGBUS, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    std::terminate_handler oldHandler = std::set_terminate(terminateHandler);
    //Set the first one as original
    if (!originalTerminateHandler) {
        originalTerminateHandler = oldHandler;
    }
}

void Utils::restoreSignalHandler() {
    setSignalHandler(SIG_DFL, originalTerminateHandler);
}

void Utils::handleTerminate() {
    //Save stacktrace
    if (dumpPath) {
        saveStackTrace(*dumpPath);
    }

    //Get exception
    std::exception_ptr e = std::current_exception();
    if (e) {
        try {
            std::rethrow_exception(e);
        } catch (std::domain_error& e) { // Inherits logic_error
            lastException = "Domain error " + std::string(e.what());
        } catch (std::invalid_argument& e) { // Inherits logic_error
            lastException = "Invalid argument " + std::string(e.what());
        } catch (std::length_error& e) { // Inherits logic_error
            lastException = "Length error " + std::string(e.what());
        } catch (std::out_of_range& e) { // Inherits logic_error
            lastException = "Out of range " + std::string(e.what());
        } catch (std::range_error& e) { // Inherits runtime_error
            lastException = "Range error " + std::string(e.what());
        } catch (std::overflow_error& e) { // Inherits runtime_error
            lastException = "Overflow error " + std::string(e.what());
        } catch (std::underflow_error& e) { // Inherits runtime_error
            lastException = "Underflow error " + std::string(e.what());
        } catch (std::logic_error& e) { // Inherits exception
            lastException = "Logic error " + std::string(e.what());
        } catch (std::runtime_error& e) { // Inherits exception
            lastException = "Runtime error " + std::string(e.what());
        } catch (std::bad_exception& e) { // Inherits exception
            lastException = "Bad exception " + std::string(e.what());
        } catch (std::exception& e) {
            lastException = "Base exception " + std::string(e.what());
        } catch (...) {
            lastException = "Unknown exception";
        }
    }

    //Pass to original handler or abort
    if (originalTerminateHandler) {
        originalTerminateHandler();
    }
    abort();
}

void Utils::handleHaltAndCatchFire(int sig) {
    /*
     * NOTE: we don't know what code can break due to unknown program state after crashing
     * so we do things according to priority and with the most dangerous stuff at the end
     */
    //We don't want recursive signal handler calls
    restoreSignalHandler();

    //Save stacktrace
    if (dumpPath) {
        saveStackTrace(*dumpPath);
    }

    //Get signal name
    std::string sigName;
    switch (sig) {
        case SIGSEGV:
            sigName = "Segmentation violation";
            break;
        case SIGABRT:
            sigName = "Abort";
            break;
        case SIGBUS:
            sigName = "Bus";
            break;
        case SIGFPE:
            sigName = "Floating-point exception";
            break;
        case SIGILL:
            sigName = "Illegal instruction";
            break;
        default:
            sigName = "Unknown " + std::to_string(sig);
            break;
    }

    //Print stacktrace
    std::list<std::string> linesStackTrace;
    if (getStackTrace(linesStackTrace)) {
        try {
            log_ptr log = Log::get(MAIN_LOG);
            log->critical("Stack Trace:");
            for (std::string& line : linesStackTrace) {
                log->critical(line);
            }
            log->critical("Signal: " + sigName);
            if (!lastException.empty()) {
                log->critical("Exception: " + lastException);
            }
        } catch (...) {
            std::cout << "Error printing with log!\n";
            std::cout << "Stack Trace:\n";
            for (std::string& line : linesStackTrace) {
                std::cout << line << "\n";
            }
            std::cout << "Signal: " << sigName << "\n";
            if (!lastException.empty()) {
                std::cout << "Exception: " << lastException << "\n";
            }
        }
    } else {
        std::cout << "Error getting stack trace!\n";
        std::cout << "Signal: " << sigName << "\n";
        if (!lastException.empty()) {
            std::cout << "Exception: " << lastException << "\n";
        }
    }

    //Show dialog
    std::ostringstream errorMessage;
    errorMessage << "Signal:\n" << sigName << "\n\n";
    if (!lastException.empty()) {
        errorMessage << "Exception:\n" << lastException << "\n\n";
    }
    if (linesStackTrace.empty()) {
        errorMessage << "Empty stack trace!\n";
    } else {
        errorMessage << "Stack trace:\n";
        join(errorMessage, BEGIN_END(linesStackTrace), "\n");
    }
    errorMessage << "\n";
    showErrorDialog(errorMessage.str(), nullptr, false, true);

    //Agur
    raise(sig);
}

void Utils::showErrorDialog(const std::string& error, const log_ptr& log, bool appendStackTrace, bool informDeveloper) {
    //Get lines from error
    std::list<std::string> lines;
    if (informDeveloper) {
        lines.emplace_back("Unexpected error occurred, please inform this to developer :(");
        lines.emplace_back(""); //Empty line
    }
    split(lines, error, "\n", false);

    //Append stack trace
    if (appendStackTrace) {
        std::list<std::string> linesStackTrace;
        bool success = getStackTrace(linesStackTrace);
        lines.emplace_back(""); //Empty line
        lines.emplace_back("Stack trace:");
        if (success && !lines.empty()) {
            std::copy(BEGIN_END(linesStackTrace), std::back_inserter(lines));
        } else {
            lines.emplace_back("Empty stack trace!");
        }
    }

    //Log if available
    if (log) {
        for (std::string& line : lines) {
            if (!line.empty()) {
                log->critical(line);
            }
        }
    }

    //Show message box
    std::string text;
    substrLines(BEGIN_END(lines), DIALOG_MAX_WIDTH);
    join(text, BEGIN_END(lines), "\n");
    SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            (std::string(GAME_TITLE) + " - Error").c_str(),
            text.c_str(),
            NULL // NOLINT
    );
}

std::string Utils::padRight(const std::string& str, std::string::size_type size) {
    if (str.size() < size) {
        return str + std::string(size - str.size(), ' ');
    }

    return std::string(str);
}

std::string Utils::padLeft(const std::string& str, std::string::size_type size) {
    if (str.size() < size) {
        return std::string(size - str.size(), ' ') + str;
    }

    return std::string(str);
}

const std::string& Utils::getInstallPath() {
    //Only create string when there is no cached one
    if (!installPath) {
        //Create a new static string for storing path
        installPath = std::make_unique<std::string>();
        char* path = SDL_GetBasePath();
        if (path) {
            //Path is valid so store it in cache
            installPath->assign(path);
            SDL_free(path);
            //Set install path to parent
            if (Utils::isFlag(FLAG_INSTALLATION_PARENT)) {
                std::string parent = Utils::getParentPath(*installPath);
                *installPath = parent;
            }
        }
    }
    return *installPath;
}

std::string Utils::getDataPath() {
    return Utils::getInstallPath() + GAME_DATA_DIR + DIR_SEP;
}

const std::string& Utils::getUserPath() {
    //Only create string when there is no cached one
    if (!userPath) {
        //Create a new static string for storing path
        userPath = std::make_unique<std::string>();
        char* userPathChar = SDL_GetPrefPath(GAME_NAME, GAME_NAME); //Both org and app are same
        if (userPathChar) {
            //Path is valid so store it in cache
            userPath->assign(userPathChar);
            SDL_free(userPathChar);

            //Set dump path too
            dumpPath = std::make_unique<std::string>();
            dumpPath->assign(*userPath + DIR_SEP + GAME_DUMP_FILE);
        }
    }

    return *userPath;
}

std::string Utils::getParentPath(const std::string& path) {
    std::string::size_type size = path.size();
    if (1 < size) {
        unsigned long pos = size - 2;
        while (0 < pos) {
            if (path[pos] == '\\' || path[pos] == '/') {
                //Got the lastmost separator that is not the last position
                return std::string(path, 0, pos + 1);
            }
            pos--;
        }
    }
    return path;
}

std::string Utils::toUpper(const std::string& text) {
    std::string result(text);
    std::string::size_type size = text.size();
    for (size_t pos = 0; pos < size; ++pos) {
        if (islower((unsigned char) text[pos])) {
            result[pos] = static_cast<char>(toupper(text[pos]));
        }
    }
    return result;
}

std::string Utils::toLower(const std::string& text) {
    std::string result(text);
    std::string::size_type size = text.size();
    for (size_t pos = 0; pos < size; ++pos) {
        if (isupper((unsigned char) text[pos])) {
            result[pos] = static_cast<char>(tolower(text[pos]));
        }
    }
    return result;
}

std::string Utils::toInternalPath(const std::string& path) {
    std::string result(path);
    std::string::size_type size = path.size();
    for (size_t pos = 0; pos < size; ++pos) {
        if (path[pos] == '\\') {
            result[pos] = '/';
        }
    }
    return result;
}

std::unique_ptr<byte_array_t> Utils::createBuffer(size_t size) {
    return std::make_unique<byte_array_t>(size);
}

bool Utils::saveStackTrace(const std::string& file) {
#if GAME_USE_BOOST
    //Save and return if saved something
    size_t amount = boost::stacktrace::safe_dump_to(file.c_str());
    return 0 < amount;
#else
    //No implementation
    if (file.empty()) {} //To shut up about unused args
    return false;
#endif
}

bool Utils::getStackTrace(std::list<std::string>& lines) {
#if GAME_USE_BOOST
    //Get current
    auto st = boost::stacktrace::stacktrace();
    //Check if failed to load
    if (st.empty()) {
        return false;
    }
    //Write lines
    for (size_t i = 0; i < st.size(); ++i) {
        //Just store the name instead of full name as we don't really care our own name
        if (i == 0) {
            lines.push_front(std::string("Utils::") + __func__ + " <- last call");
            continue;
        }

        //Pull the stacktrace info
        std::string line = boost::stacktrace::detail::to_string(&st.as_vector()[i], 1);
        std::string::size_type size = line.size();
        if (10 <= size) {
            //Remove the start number and end newline
            line = line.substr(4, size - 5);
            lines.push_front(line);
        }
    }
    return !lines.empty();
#else
    //No implementation
    if (lines.empty()) {} //To shut up about unused args
    return false;
#endif
}

bool Utils::listDirectory(const std::string& dirPath, std::list<std::string>& dirPaths) {
#if GAME_USE_BOOST
    using namespace boost::filesystem;

    //Check path
    if (!exists(dirPath) || !is_directory(dirPath)) {
        return false;
    }

    //Iterate and add all content of dir to paths
    directory_iterator endIter; // default construction yields past-the-end
    for (directory_iterator dirIter(dirPath); dirIter != endIter; ++dirIter) {
        dirPaths.push_back(dirIter->path().leaf().string());
    }
    return true;
#else
    //No implementation
    if (dirPath.empty() && dirPaths.empty()) {} //To shut up about unused args
    return false;
#endif
}


/**
 * Flips the buffer lines in Y axis
 *
 * @param width of lines in bytes
 * @param lines of buffer
 * @param data to flip
 */
std::unique_ptr<byte_array_t> Utils::bufferFlipY(const byte_array_t data, size_t width, size_t height) {
    //Copy first line before shifting all
    std::unique_ptr<byte_array_t> tmp = createBuffer(width * height);
    for (size_t i = 0; i < height; ++i) {
        memcpy(tmp.get() + i * width, data + (height - i - 1) * width, width);
    }
    return tmp;
}

void Utils::getRootPaths(const std::string& name, std::vector<std::string>& roots) {
    roots.emplace_back(Utils::getInstallPath() + name);                       //Installation directory
    roots.emplace_back(name);                                                 //Current directory
}
