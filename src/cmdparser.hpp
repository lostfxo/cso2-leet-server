#ifndef __CMD_PARSER_H_
#define __CMD_PARSER_H_

#include <string_view>
#include <vector>

class CmdParser
{
public:
    CmdParser(int argc, char** argv);
    ~CmdParser() = default;

    std::string_view GetOption(std::string_view targetOpt) const noexcept;
    unsigned long GetUintOption(std::string_view targetOpt) const noexcept;
    std::string_view GetByIndex(std::size_t targetIndex) const noexcept;
    bool HasOption(std::string_view targetOpt) const noexcept;

    inline std::size_t GetArgsNum() const noexcept;

private:
    std::vector<std::string_view> m_vArgs;
};

inline std::size_t CmdParser::GetArgsNum() const noexcept
{
    return this->m_vArgs.size();
}

#endif  // __CMD_PARSER_H_
