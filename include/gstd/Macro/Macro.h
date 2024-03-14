#ifndef GSTD_MACRO_H
#define GSTD_MACRO_H

/*
 *
 * Compiler macros
 *
 */

#define GSTD_COMPILER_BASE_ID 0

#define GSTD_COMPILER_UNKNOWN_ID (GSTD_COMPILER_BASE_ID)
#define GSTD_COMPILER_MSVC_ID    (GSTD_COMPILER_BASE_ID + 1)
#define GSTD_COMPILER_MINGW_ID   (GSTD_COMPILER_BASE_ID + 2)

#if defined(_MSC_VER)
    #define GSTD_COMPILER_MSVC
#elif defined(__MINGW32__) || defined(__MINGW64__)
    #define GSTD_COMPILER_MINGW
#endif

#if defined(GSTD_COMPILER_MSVC)
    #define GSTD_COMPILER_ID GSTD_COMPILER_MSVC_ID
#elif defined(GSTD_COMPILER_MINGW)
    #define GSTD_COMPILER_ID GSTD_COMPILER_MINGW_ID
#else
    #define GSTD_COMPILER_ID GSTD_COMPILER_UNKNOWN_ID

    #warning "[warning] gstd/Macro/Macro.h: Compiler not detected!"
#endif

/*
 *
 * CPP standard macros
 *
 */

#define GSTD_CPP_BASE_ID 0

#define GSTD_CPP_20_ID      20
#define GSTD_CPP_UNKNOWN_ID 0

#define GSTD_CPP_20

#if defined(GSTD_CPP_20)
    #define GSTD_CPP_ID GSTD_CPP_20_ID
#else
    #define GSTD_CPP_ID GSTD_CPP_UNKNOWN_ID

    #warning "[warning] gstd/Macro/Macro.h: CPP standard not detected!"
#endif

/*
 *
 * OS Macros
 *
 */

#define GSTD_OS_WINDOWS_ID 1
#define GSTD_OS_UNKNOWN_ID 0

#if defined(WIN32) || defined(WIN64)
    #define GSTD_OS_WINDOWS
#endif

#if defined(GSTD_OS_WINDOWS)
    #define GSTD_OS_ID GSTD_OS_WINDOWS_ID
#else
    #define GSTD_OS_ID GSTD_OS_UNKNOWN_ID

    #warning "[warning] gstd/Macro/Macro.h: OS not detected!"
#endif

/**
 * Keyword 'explicit'
 */
#define GSTD_EXPLICIT explicit

/**
 * Keyword 'noexcept'
 */
#define GSTD_NOEXCEPT noexcept

/**
 * Keyword 'constexpr'
 */
#define GSTD_CONSTEXPR constexpr

/**
 * Keyword 'inline'
 */
#define GSTD_INLINE inline

/**
 * Keyword 'consteval'
 */
#define GSTD_CONSTEVAL consteval

/**
 * Attribute macro
 */
#define GSTD_ATTRIBUTE(name) [[name]]

/**
 * Attribute 'noreturn'
 */
#define GSTD_NORETURN GSTD_ATTRIBUTE(noreturn)

/**
 * File name macro
 */
#define GSTD_FILE __FILE__

/**
 * Function name macro
 */
#define GSTD_FUNCTION __FUNCTION__

/**
 * Line number macro
 */
#define GSTD_LINE __LINE__

#if defined(__cpp_lib_char8_t)
    #define GSTD_HAS_CHAR8T
#endif

/**
 * Column number macro
 * @todo Replace zero to any macro
 */
#define GSTD_COLUMN 0

//namespace gstd {
//
//    enum class CompilerKind {
//        MSVC,
//        MinGW,
//        Unknown
//    };
//
//    enum class CPPStandardKind {
//        CPP20,
//        Unknown
//    };
//
//    enum class OSKind {
//        Windows,
//        Unknown
//    };
//
//    GSTD_CONSTEXPR auto CompilerName() -> const char * {
//        switch (CompilerId) {
//            case CompilerKind::MSVC:
//                return "MSVC";
//            case CompilerKind::MinGW:
//                return "MinGW";
//            case CompilerKind::Unknown:
//                return "Unknown";
//        }
//
//        return "Invalid";
//    }
//
//    GSTD_CONSTEXPR auto OSName() -> const char * {
//        switch (OSId) {
//            case OSKind::Windows:
//                return "Windows";
//            case OSKind::Unknown:
//                return "Unknown";
//        }
//
//        return "Invalid";
//    }
//
//}

#endif //GSTD_MACRO_H
