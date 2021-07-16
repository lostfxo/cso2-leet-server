#include "packetlogger.hpp"

#include "util/fs.hpp"
#include "util/log.hpp"

static fs::path g_InPath;
static fs::path g_OutPath;

inline void WriteToFile(const std::span<const std::uint8_t> buffer,
                        const fs::path& path)
{
    auto convertedPath = path.generic_string();
    WriteBufferToFile(buffer, convertedPath);
}

inline void ClearDirectory(const fs::path& path)
{
    for (auto& p : fs::directory_iterator(path))
    {
        fs::remove(p);
    }
}

inline std::string PadNumber(std::uint32_t n)
{
    std::string res;

    if (n > 9)
    {
        res += '0';
    }

    res += std::to_string(n);
    return res;
}

void PacketLogger::Init()
{
    Log::Info("[PacketLogger::Init] packet logging is enabled\n");

    auto curPath = fs::current_path();
    g_InPath = curPath / "packets/in";
    g_OutPath = curPath / "packets/out";

    Log::Info("[PacketLogger::Init] using {} as in path\n",
              g_InPath.generic_string());
    Log::Info("[PacketLogger::Init] using {} as out path\n",
              g_OutPath.generic_string());

    fs::create_directories(g_InPath);
    fs::create_directories(g_OutPath);

    if (fs::is_empty(g_InPath) == false)
    {
        Log::Warning("[PacketLogger::Init] cleaning {}\n",
                     g_InPath.generic_string());
        ClearDirectory(g_InPath);
    }

    if (fs::is_empty(g_OutPath) == false)
    {
        Log::Warning("[PacketLogger::Init] cleaning {}\n",
                     g_OutPath.generic_string());
        ClearDirectory(g_OutPath);
    }
}

void PacketLogger::OnInPacket(std::string_view connUuid, std::uint32_t seq,
                              std::uint32_t packetId,
                              const std::span<const std::uint8_t> packetData)
{
    auto writePath =
        g_InPath / fmt::format("{}_{}-{}", connUuid, PadNumber(seq), packetId);
    WriteToFile(packetData, writePath);
}

void PacketLogger::OnOutPacket(std::string_view connUuid, std::uint32_t seq,
                               std::uint32_t packetId,
                               const std::span<const std::uint8_t> packetData)
{
    auto writePath =
        g_OutPath / fmt::format("{}_{}-{}", connUuid, PadNumber(seq), packetId);
    WriteToFile(packetData, writePath);
}
