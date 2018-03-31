//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_UTILS_H
#define OPENE2140_UTILS_H

#include <csignal>
#include <list>
#include "log.h"

#define DIALOG_MAX_WIDTH 150

class Utils {
private:
    /** Debug flag */
    static bool debug;

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
    static std::basic_string<char> checkSDLError(const log_ptr& log = nullptr);

    /**
     * Converts value with n precision to string using ostream
     *
     * @tparam T type of value
     * @param value to write
     * @param n of precission
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
    static void showErrorDialog(const std::string& error, const log_ptr& log, bool appendStackTrace);

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
};

#endif //OPENE2140_UTILS_H
