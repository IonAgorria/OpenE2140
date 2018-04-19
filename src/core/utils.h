//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_UTILS_H
#define OPENE2140_UTILS_H

#include "config.h"
#include <csignal>
#include <list>
#include "log.h"

#define DIALOG_MAX_WIDTH 150

/**
 * Contains static utilities
 */
class Utils {
private:
    /** Debug flag */
    static bool debug;
    /** Cached install path */
    static std::string* installPath;
    /** Cached user path */
    static std::string* userPath;

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
     * Converts value with n precision to string using ostream
     *
     * @tparam T type of value
     * @param value to write
     * @param n of precision
     */
    template<typename T>
    static std::string toStringPrecision(const T& value, int n);

    /**
     * Saves current stack trace to file
     *
     * @return if save was successful
     */
    static bool saveStackTrace(const char* file);

    /**
     * Writes current stack trace to out stream
     *
     * @param lines to use
     * @return if get was successful
     */
    static bool getStackTrace(std::list<std::string>& lines);

    /**
     * Set custom signal handlers
     */
    static void setSignalHandler(__sighandler_t signal_handler);

    /**
     * Set default signal handlers
     */
    static void restoreSignalHandler();

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
     */
    static void showErrorDialog(const std::string& error, const log_ptr log, bool appendStackTrace);

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
    static Stream& join(Stream& stream, const Iterator& begin, const Iterator& end, const Separator& glue);

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
    static std::string& join(std::string& string, const Iterator& begin, const Iterator& end, const Separator& glue);

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
    static void split(T& elements, const std::string& string, const Separator& separator, bool trimEmpty);

    /**
     * Makes a substr to all elements in iterator
     *
     * @tparam Iterator type
     * @param begin iterator
     * @param end iterator
     * @param size of each element
     */
    template <typename Iterator>
    static void substrLines(const Iterator& begin, const Iterator& end, std::string::size_type size);

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
     * Lists all content of specified path and stores on provided list
     *
     * @param dirPath to list
     * @param callback when a name is found
     * @return if success
     */
    static bool listDirectory(const std::string& dirPath, std::list<std::string>& dirPaths);

    /**
     * Converts text uppercase
     *
     * @param text to change
     * @return uppercase text
     */
    static std::string toUpper(const std::string text);

    /**
     * Converts text lowercase
     *
     * @param text to change
     * @return lowercase text
     */
    static std::string toLower(const std::string text);

    static std::unique_ptr<byte[]> createBuffer(const size_t size);
};

#endif //OPENE2140_UTILS_H
