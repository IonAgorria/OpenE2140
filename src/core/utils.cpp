//
// Created by Ion Agorria on 22/03/18
//
#include <cstdio>
#include <iostream>
#include <csignal>
#include <iomanip>
#include <list>
#include "SDL_filesystem.h"
#include "SDL_quit.h"
#include "SDL_messagebox.h"
#include "utils.h"
#include "config.h"

//Attempt to load boost libs
#if HAS_BOOST
#   include "boost/stacktrace.hpp"
#   include "boost/filesystem.hpp"
#endif

//Default static variable values
bool Utils::debug = false;
std::string* Utils::installPath = nullptr;
std::string* Utils::userPath = nullptr;

void Utils::setDebug(bool value) {
    debug = value;
}

bool Utils::isDebug() {
    return debug;
}

std::string Utils::checkSDLError(const log_ptr log) {
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

template <typename T>
std::string Utils::toStringPrecision(const T& value, int n)
{
    std::ostringstream out;
    out << std::fixed << std::showpoint << std::setprecision(n) << value;
    return out.str();
}

bool Utils::saveStackTrace(const char* file) {
#if HAS_BOOST
    //Save and return if saved something
    size_t amount = boost::stacktrace::safe_dump_to(file);
    return 0 < amount;
#else
    return false; //No implementation
#endif
}

bool Utils::getStackTrace(std::list<std::string>& lines) {
#if HAS_BOOST
    //Get current
    auto st = boost::stacktrace::stacktrace();
    //Check if failed to load
    if (st.empty()) {
        return false;
    }
    //Write lines
    for (unsigned int i = 0; i < st.size(); ++i) {
        //Just store the name instead of full name as we don't really care our own name
        if (i == 0) {
            lines.push_back(std::string("Utils::") + __func__ + " <- last call");
            continue;
        }

        //Pull the stacktrace info
        std::string line = boost::stacktrace::detail::to_string(&st.as_vector()[i], 1);
        if (!line.empty()) {
            //Remove the start number and end newline
            line = line.substr(4, line.size() - 5);
            lines.push_back(line);
        }
    }
    return !lines.empty();
#else
    return false; //No implementation
#endif
}

void Utils::setSignalHandler(__sighandler_t signal_handler) {
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGBUS, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGILL, signal_handler);
}

void Utils::restoreSignalHandler() {
    setSignalHandler(SIG_DFL);
}

void Utils::handleHaltAndCatchFire(int sig) {
    /*
     * NOTE: we don't know what code can break due to unknown program state after crashing
     * so we do things according to priority and with the most dangerous stuff at the end
     */
    //We don't want recursive signal handler calls
    restoreSignalHandler();

    //Save stacktrace
    saveStackTrace(GAME_DUMP_FILE);

    //Get signal name
    std::string sigName;
    switch (sig) {
        case SIGSEGV:
            sigName = "Signal: Segmentation violation";
            break;
        case SIGABRT:
            sigName = "Signal: Abort";
            break;
        case SIGBUS:
            sigName = "Signal: Bus";
            break;
        case SIGFPE:
            sigName = "Signal: Floating-point exception";
            break;
        case SIGILL:
            sigName = "Signal: Illegal instruction";
            break;
        default:
            sigName = "Signal: Unknown " + std::to_string(sig);
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
            log->critical(sigName);
        } catch (std::exception& e) {
            std::cout << "Error printing with log! " << e.what() << "\n";
            std::cout << "Stack Trace:\n";
            for (std::string& line : linesStackTrace) {
                std::cout << line << "\n";
            }
            std::cout << sigName << "\n";
        }
    } else {
        std::cout << "Error getting stack trace!\n";
        std::cout << sigName << "\n";
    }

    //Show dialog
    std::ostringstream errorMessage;
    errorMessage << sigName << "\n\n";
    if (linesStackTrace.empty()) {
        errorMessage << "Empty stack trace!\n";
    } else {
        errorMessage << "Stack trace:\n";
        join(errorMessage, BEGIN_END(linesStackTrace), "\n");
    }
    showErrorDialog(errorMessage.str(), nullptr, false);

    //Agur
    raise(sig);
}

void Utils::showErrorDialog(const std::string& error, const log_ptr log, bool appendStackTrace) {
    //Get lines from error
    std::list<std::string> lines;
    lines.emplace_back("Unexpected error occurred, please inform this to developer :(");
    lines.emplace_back(""); //Empty line
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

template <typename Stream, typename Iterator, typename Separator>
Stream& Utils::join(Stream& stream, const Iterator& begin, const Iterator& end, const Separator& glue) {
    for (Iterator i = begin; i != end; ++i) {
        if (i != begin) stream << glue;
        stream << *i;
    }
    return stream;
}

template <typename Iterator, typename Separator>
std::string& Utils::join(std::string& string, const Iterator& begin, const Iterator& end, const Separator& glue) {
    for (Iterator i = begin; i != end; ++i) {
        if (i != begin) string += glue;
        string += *i;
    }
    return string;
}

template <typename T, typename Separator>
void Utils::split(T& elements, const std::string& string, const Separator& separator, bool trimEmpty) {
    std::string::size_type pos, lastPos = 0, length = string.length();
    using value_type = typename T::value_type;
    using size_type  = typename T::size_type;

    while(lastPos < length + 1)
    {
        //Find separator string
        pos = string.find(separator, lastPos);
        if (pos == std::string::npos)
        {
            //Nothing found, go to end
            pos = length;
        }

        //Add element
        if (pos != lastPos || !trimEmpty) {
            elements.push_back(value_type(string.data() + lastPos, (size_type) pos - lastPos));
        }

        //Next
        lastPos = pos + 1;
    }
}

template <typename Iterator>
void Utils::substrLines(const Iterator& begin, const Iterator& end, std::string::size_type size) {
    for (Iterator i = begin; i != end; ++i) {
        //This pulls the line from iterator, makes substr and sets back the content as it's a reference
        std::string& line = *i;
        line = line.substr(0, std::min(line.size(), size));
    }
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
    if (installPath == nullptr) {
        //Create a new static string for storing path
        installPath = new std::string();
        char* path = SDL_GetBasePath();
        if (path) {
            //Path is valid so store it in cache
            installPath->assign(path);
            SDL_free(path);
        }
    }
    return *installPath;
}

const std::string& Utils::getUserPath() {
    //Only create string when there is no cached one
    if (userPath == nullptr) {
        //Create a new static string for storing path
        userPath = new std::string();
        char* path = SDL_GetPrefPath(GAME_NAME, GAME_NAME); //Both org and app are same
        if (path) {
            //Path is valid so store it in cache
            userPath->assign(path);
            SDL_free(path);
        }
    }
    return *userPath;
}

bool Utils::listDirectory(const std::string& dirPath, std::list<std::string>& dirPaths) {
#if HAS_BOOST
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
    return false; //No implementation
#endif
}

std::string Utils::toUpper(const std::string text) {
    std::string result(text.size(), '\0');
    for (int pos = 0; text[pos] != '\0'; ++pos) {
        if (islower((unsigned char) text[pos])) {
            result[pos] = static_cast<char>(toupper(text[pos]));
        }
    }

    return text;
}

std::string Utils::toLower(const std::string text) {
    std::string result(text.size(), '\0');
    for (int pos = 0; text[pos] != '\0'; ++pos) {
        if (isupper((unsigned char) text[pos])) {
            result[pos] = static_cast<char>(tolower(text[pos]));
        }
    }

    return text;
}

std::unique_ptr<byte[]> Utils::createBuffer(const size_t size) {
    return std::move(std::make_unique<byte[]>(size));
}
