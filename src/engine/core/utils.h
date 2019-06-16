//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_UTILS_H
#define OPENE2140_UTILS_H

#include <iomanip>
#include <iostream>
#include <sstream> //MacOS requires this to avoid implicit templates in string streams
#include <csignal>
#include <list>
#include "common.h"
#include "io/log.h"

#define DIALOG_MAX_WIDTH 150

typedef void (*sighandler) (int);

/**
 * Contains static utilities
 */
class Utils {
private:
    /** Debug flag */
    static bool debug;
    /** Cached install path */
    static std::unique_ptr<std::string> installPath;
    /** Cached user path */
    static std::unique_ptr<std::string> userPath;
    /** Cached dump path */
    static std::unique_ptr<std::string> dumpPath;
    /** Original terminate handler */
    static std::terminate_handler originalTerminateHandler;
    /*** Last exception info */
    static std::string lastException;
public:
    /**
     * Set debug mode
     *
     * @param value to set
     */
    static void setDebug(bool value);

    /**
     * @return if debug mode is on
     */
    static bool isDebug();

    /**
     * Checks if SDL has any error
     *
     * @param log to write the error or null to ignore
     * @return the produced error or string representing the lack of error
     */
    static std::string checkSDLError(const log_ptr log = nullptr);

    /**
     * Checks if OpenGL has any error
     *
     * @param log to write the error or null to ignore
     * @return the produced error or string representing the lack of error
     */
    static std::string checkGLError(const log_ptr log = nullptr);

    /**
     * Checks if SDL and/or OpenGL has any error and concatenate them with newline if several errors occur
     *
     * @param log to write the error or null to ignore
     * @return the produced error or string representing the lack of error
     */
    static std::string checkAnyError(const log_ptr log = nullptr);

    /**
     * Converts value with n precision to string using ostream
     *
     * @tparam T type of value
     * @param value to write
     * @param n of precision
     * @return string
     */
    template<typename T>
    static std::string toStringPrecision(const T& value, int n) {
        std::ostringstream out;
        out << std::fixed << std::showpoint << std::setprecision(n) << value;
        return out.str();
    }

    /**
     * Converts value to hex string
     *
     * @tparam T type of value
     * @param value to write
     * @return string
     */
    template<typename T>
    static std::string toStringHex(const T& value) {
        std::ostringstream out;
        out << std::hex << value;
        return out.str();
    }

    /**
     * Returns if a and starts with b
     *
     * @param string to check
     * @param start to compare
     * @return
     */
    static bool startsWith(const std::string& string, const std::string& start);

    /**
     * Returns if a ends with b
     *
     * @param string to check
     * @param end to compare
     * @return
     */
    static bool endsWith(const std::string& string, const std::string& end);

    /**
     * Set custom signal handlers
     */
    static void setSignalHandler(sighandler signalHandler, std::terminate_handler terminateHandler);

    /**
     * Set default signal handlers
     */
    static void restoreSignalHandler();

    /**
     * Called when program must terminate
     */
    static void handleTerminate();

    /**
     * Called when signal occurs
     *
     * @param sig number
     */
    static void handleHaltAndCatchFire(int sig);

    /**
     * Shows error
     *
     * @param error to show
     * @param log to use to show error along dialog, null ptr to ignore log
     * @param appendStackTrace to append stack trace at bottom
     * @param informDeveloper to show inform developer text
     */
    static void showErrorDialog(const std::string& error, const log_ptr log, bool appendStackTrace, bool informDeveloper);

    /**
     * Joins each element between iterators to stream using glue
     *
     * @param stream to output to
     * @param begin iterator
     * @param end iterator
     * @param glue for each element
     *
     * Source: https://stackoverflow.com/questions/1430757/c-vector-to-string
     */
    template <typename Stream, typename Iterator, typename Separator>
    static Stream& join(Stream& stream, const Iterator& begin, const Iterator& end, const Separator& glue) {
        for (Iterator i = begin; i != end; ++i) {
            if (i != begin) stream << glue;
            stream << *i;
        }
        return stream;
    }

    /**
     * Joins each element between iterators and adds to element using glue
     *
     * @tparam Iterator type of iterator
     * @tparam Separator type of iterator
     * @param stream to output to
     * @param begin iterator
     * @param end iterator
     * @param glue for each element
     */
    template <typename Iterator, typename Separator>
    static std::string& join(std::string& string, const Iterator& begin, const Iterator& end, const Separator& glue) {
        for (Iterator i = begin; i != end; ++i) {
            if (i != begin) string += glue;
            string += *i;
        }
        return string;
    }

    /**
     * Splits string with separator and stores on elements
     *
     * @tparam T type of value
     * @param elements collection to write
     * @param string to split
     * @param separator to use
     * @param trimEmpty to not add empty elements
     *
     * Source: https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
     */
    template <typename T, typename Separator>
    static void split(T& elements, const std::string& string, const Separator& separator, bool trimEmpty) {
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

    /**
     * Makes a substr to all elements in iterator
     *
     * @tparam Iterator type
     * @param begin iterator
     * @param end iterator
     * @param size of each element
     */
    template <typename Iterator>
    static void substrLines(const Iterator& begin, const Iterator& end, std::string::size_type size) {
        for (Iterator i = begin; i != end; ++i) {
            //This pulls the line from iterator, makes substr and sets back the content as it's a reference
            std::string& line = *i;
            line = line.substr(0, std::min(line.size(), size));
        }
    }

    /**
     * Does right padding if is smaller than size
     *
     * @param str to pad
     * @param size of string
     * @return padded str
     */
    static std::string padRight(const std::string& str, std::string::size_type size);

    /**
     * Does left padding if is smaller than size
     *
     * @param str to pad
     * @param size of string
     * @return padded str
     */
    static std::string padLeft(const std::string& str, std::string::size_type size);

    /**
     * Program base path where the installation files are stored or empty string if path is not available.
     * Path's are guaranteed to end with dir separator.
     *
     * @return path string or empty string
     */
    static const std::string& getInstallPath();

    /**
     * Program user path where the user files are stored or empty string if path is not available.
     * Path's are guaranteed to end with dir separator.
     *
     * @return path string or empty string
     */
    static const std::string& getUserPath();

    /**
     * Gets the parent path of provided path
     *
     * @param path to get parent path from
     * @return parent path or original path if no parent was found
     */
    static std::string getParentPath(const std::string& path);

    /**
     * Converts text uppercase
     *
     * @param text to change
     * @return uppercase text
     */
    static std::string toUpper(const std::string& text);

    /**
     * Converts text lowercase
     *
     * @param text to change
     * @return lowercase text
     */
    static std::string toLower(const std::string& text);

    /**
     * Converts path to internal format for unique identification
     *
     * @param path to change
     * @return internal path
     */
    static std::string toInternalPath(const std::string& path);

    /**
     * Creates a buffer in memory with specified size
     *
     * @param size of buffer
     * @return unique pointer of buffer
     */
    static std::unique_ptr<byte_array_t> createBuffer(const size_t size);

    /**
     * Saves current stack trace to file
     *
     * @return if save was successful
     */
    static bool saveStackTrace(const std::string& file);

    /**
     * Writes current stack trace to out stream
     *
     * @param lines to use
     * @return if get was successful
     */
    static bool getStackTrace(std::list<std::string>& lines);

    /**
     * Lists all content of specified path and stores on provided list
     *
     * @param dirPath to list
     * @param callback when a name is found
     * @return if success
     */
    static bool listDirectory(const std::string& dirPath, std::list<std::string>& dirPaths);

    /**
     * Flips the lines in buffer vertically
     *
     * @param width of each line in buffer
     * @param height lines of buffer
     * @param data pointer to data
     * @return flipped data
     */
    static std::unique_ptr<byte_array_t> bufferFlipY(const byte_array_t data, unsigned int width, unsigned int height);

    /**
     * Moves unique pointer from vector, returns it and erases the vector position
     *
     * @tparam T type inside ptrs of vector
     * @param vector vector to manipulate
     * @param element element pointer to remove
     * @return element unique pointer
     */
    template<typename T>
    static std::unique_ptr<T> erasePointerFromVector(std::vector<std::unique_ptr<T>>& vector, T* element) {
        std::unique_ptr<T> result;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            std::unique_ptr<T>& pointer = *it;
            if (pointer.get() == element) {
                result = std::move(pointer);
                vector.erase(it);
                break;
            }
        }
        return result;
    }

    /**
     * Removes element from vector
     *
     * @tparam T type inside vector
     * @param vector vector to manipulate
     * @param element element pointer to remove
     * @return bool if removed successfully
     */
    template<typename T>
    static bool eraseElementFromVector(std::vector<T>& vector, T element) {
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            if (*it == element) {
                vector.erase(it);
                return true;
            }
        }
        return false;
    }
};

#endif //OPENE2140_UTILS_H
