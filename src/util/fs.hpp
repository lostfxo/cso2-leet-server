#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <span>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

inline std::pair<bool, std::vector<std::uint8_t>> ReadFileToBuffer(
    std::string_view filename, std::uint64_t readLength = 0)
{
    if (fs::is_regular_file(filename.data()) == false)
    {
        return { false, {} };
    }

    std::ifstream is(filename.data(), std::ios::binary | std::ios::ate);

    if (is.is_open() == false)
    {
        return { false, {} };
    }

    if (readLength == 0)
    {
        std::uint64_t iFileSize = is.tellg();
        readLength = iFileSize;
    }

    std::vector<std::uint8_t> res(readLength);

    is.seekg(std::ios::beg);
    is.read(reinterpret_cast<char*>(res.data()), readLength);

    return { true, std::move(res) };
}

inline bool WriteBufferToFile(const std::span<const std::uint8_t> buff,
                              std::string_view filename)
{
    std::ofstream os(filename.data(), std::ios::binary);

    if (os.is_open() == false)
    {
        return false;
    }

    os.write(reinterpret_cast<const char*>(buff.data()), buff.size_bytes());

    return true;
}
