#include "cmdparser.hpp"

#include <algorithm>
#include <charconv>

CmdParser::CmdParser(int argc, char** argv)
{
    char** begin = argv;
    char** end = argv + argc;

    std::vector<std::string_view> vArgs;

    std::for_each(begin, end, [&](const char* cur) { vArgs.push_back(cur); });
    this->m_vArgs = std::move(vArgs);
}

std::string_view CmdParser::GetOption(std::string_view targetOpt) const noexcept
{
    auto it = std::find(this->m_vArgs.begin(), this->m_vArgs.end(), targetOpt);

    if (it != this->m_vArgs.end() && ++it != this->m_vArgs.end())
    {
        return *it;
    }

    return {};
}

unsigned long CmdParser::GetUintOption(
    std::string_view targetOpt) const noexcept
{
    unsigned long result = 0;
    auto option = this->GetOption(targetOpt);

    auto convertRes =
        std::from_chars(option.data(), option.data() + option.size(), result);

    return convertRes.ec == std::errc::invalid_argument ? 0 : result;
}

std::string_view CmdParser::GetByIndex(std::size_t targetIndex) const noexcept
{
    return this->m_vArgs.at(targetIndex);
}

bool CmdParser::HasOption(std::string_view targetOpt) const noexcept
{
    return std::find(this->m_vArgs.begin(), this->m_vArgs.end(), targetOpt) !=
           this->m_vArgs.end();
}
