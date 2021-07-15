#include "util/log.hpp"

#include <filesystem>
#include <fstream>
#include "util/number.hpp"
#include <iostream>
#include <string>

namespace fs = std::filesystem;

LogVerbosity Log::m_Verbosity = LogVerbosity::Info;

LogVerbosity StringToLogVerbosity(std::string_view str)
{
    if (str == "debug")
    {
        return LogVerbosity::Debug;
    }
    else if (str == "warning")
    {
        return LogVerbosity::Warning;
    }
    else if (str == "info")
    {
        return LogVerbosity::Info;
    }
    else if (str == "error")
    {
        return LogVerbosity::Error;
    }

    return LogVerbosity::Unknown;
}

void WriteToConsole(const std::string& msg)
{
    std::cout << msg;
}

void WriteToFile(const std::string& msg)
{
    fs::path objLogPath = fs::current_path() / "server.log";

    std::ofstream os(objLogPath.c_str(), std::ios::app);

    if (os.is_open() == true)
    {
        os.write(msg.data(), util::FastNarrow<std::uint32_t>(msg.length()));
    }

    os.close();
}

void Log::PrintDebug(const char* format, fmt::format_args args)
{
    if (m_Verbosity > LogVerbosity::Debug)
    {
        return;
    }

    std::string msg = "[Debug] " + fmt::vformat(format, args);
    WriteToConsole(msg);
}

void Log::PrintInfo(const char* format, fmt::format_args args)
{
    if (m_Verbosity > LogVerbosity::Info)
    {
        return;
    }

    std::string msg = "[Info] " + fmt::vformat(format, args);
    WriteToConsole(msg);
}

void Log::PrintWarning(const char* format, fmt::format_args args)
{
    if (m_Verbosity > LogVerbosity::Warning)
    {
        return;
    }

    std::string msg = "[Warn] " + fmt::vformat(format, args);
    WriteToConsole(msg);
}

void Log::PrintError(const char* format, fmt::format_args args)
{
    if (m_Verbosity > LogVerbosity::Error)
    {
        return;
    }

    std::string msg = "[Error] " + fmt::vformat(format, args);
    WriteToConsole(msg);
}
