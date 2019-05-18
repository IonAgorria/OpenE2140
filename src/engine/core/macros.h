//
// Created by Ion Agorria on 2/12/18
//
#ifndef OPENE2140_MACROS_H
#define OPENE2140_MACROS_H

//Source https://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value
#define QUOTE(val) #val
#define STR(val) QUOTE(val)

/** Macro for expanding begin end */
#define BEGIN_END(container) (container).begin(), (container).end()

/** Dir separator */
#ifdef OPENE2140_IS_WINDOWS
#    define DIR_SEP "\\"
#else
#    define DIR_SEP "/"
#endif

/** Removes copy operators */
#define NON_COPYABLE(T) \
      T(const T&) = delete; \
      void operator=(const T &t) = delete;

/** Removes move operators */
#define NON_MOVABLE(T) \
      T(T&&) = delete; \
      void operator=(const T &&t) = delete;

/** Removes copy and move operators */
#define NON_COPYABLE_NOR_MOVABLE(T) \
      NON_COPYABLE(T) \
      NON_MOVABLE(T)

/** Stores type name */
#define TYPE_NAME(T) \
    const char* type_name = #T;

#endif //OPENE2140_MACROS_H
