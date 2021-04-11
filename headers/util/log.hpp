#ifndef __UTIL_LOG_H_
#define __UTIL_LOG_H_

#include <fmt/format.h>
#include <string_view>

#ifdef LOG_DEBUG
#include <cstdlib>
#define _Assert_STR(x) #x
#define Assert(x)                                                            \
    if (!(x))                                                                \
    {                                                                        \
        Log::Error("Assertion failed ({}) at file {}, {}\n", _Assert_STR(x), \
                   __FILE__, __LINE__);                                      \
        std::abort();                                                        \
    }
#else
#define Assert(x)
#endif

enum class LogVerbosity
{
    Unknown = -1,
    Debug = 0,
    Info,
    Warning,
    Error,
};

LogVerbosity StringToLogVerbosity(std::string_view str);

class Log
{
public:
    template <typename... Args>
    inline static void Debug(std::string_view format, const Args&... args)
    {
        Log::PrintDebug(format.data(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    inline static void Info(std::string_view format, const Args&... args)
    {
        Log::PrintInfo(format.data(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    inline static void Warning(std::string_view format, const Args&... args)
    {
        Log::PrintWarning(format.data(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    inline static void Error(std::string_view format, const Args&... args)
    {
        Log::PrintError(format.data(), fmt::make_format_args(args...));
    }

    inline static void SetVerbosity(LogVerbosity newVerbosity)
    {
        m_Verbosity = newVerbosity;
    }

private:
    static void PrintDebug(const char* format, fmt::format_args args);
    static void PrintInfo(const char* format, fmt::format_args args);
    static void PrintWarning(const char* format, fmt::format_args args);
    static void PrintError(const char* format, fmt::format_args args);

    static LogVerbosity m_Verbosity;

    // disable constructors and copy assignments
    // (move assignments are disabled implicitally)
private:
    Log() = delete;
    ~Log() = delete;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
};

#endif  // __UTIL_LOG_H_
