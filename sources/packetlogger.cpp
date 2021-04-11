#include "packetlogger.hpp"

#include "util/log.hpp"

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

PacketLogger::PacketLogger()
{
    auto curPath = fs::current_path();
    this->m_InPath = curPath / "packets/in";
    this->m_OutPath = curPath / "packets/out";

    Log::Info("PacketLogger: using {} as in path\n",
              this->m_InPath.generic_string());
    Log::Info("PacketLogger: using {} as out path\n",
              this->m_OutPath.generic_string());

    fs::create_directories(this->m_InPath);
    fs::create_directories(this->m_OutPath);

    if (fs::is_empty(this->m_InPath) == false)
    {
        Log::Warning("PacketLogger: cleaning {}\n",
                     this->m_InPath.generic_string());
        ClearDirectory(this->m_InPath);
    }

    if (fs::is_empty(this->m_OutPath) == false)
    {
        Log::Warning("PacketLogger: cleaning {}\n",
                     this->m_OutPath.generic_string());
        ClearDirectory(this->m_OutPath);
    }
}

void PacketLogger::OnInPacket(std::string_view connUuid, std::uint32_t seq,
                              std::uint32_t packetId,
                              const std::span<const std::uint8_t> packetData) const
{
    auto writePath = this->m_InPath / fmt::format("{}_{}-{}", connUuid,
                                                  PadNumber(seq), packetId);
    WriteToFile(packetData, writePath);
}

void PacketLogger::OnOutPacket(std::string_view connUuid, std::uint32_t seq,
                               std::uint32_t packetId,
                               const std::span<const std::uint8_t> packetData) const
{
    auto writePath = this->m_OutPath / fmt::format("{}_{}-{}", connUuid,
                                                   PadNumber(seq), packetId);
    WriteToFile(packetData, writePath);
}
