#ifndef __BUFFERVIEW_H_
#define __BUFFERVIEW_H_

#include <cstdint>
#include <span>

#include "util/binary.hpp"

class BufferView
{
public:
    BufferView(const std::span<const std::uint8_t> data);

    template <typename DATA_TYPE>
    [[nodiscard]] DATA_TYPE Read(
        bool bigEndian = false) requires std::is_arithmetic<DATA_TYPE>::value
    {
        DATA_TYPE result;
        this->ReadImpl(
            { reinterpret_cast<std::uint8_t*>(&result), sizeof(DATA_TYPE) });
        return bigEndian ? ReverseEndianness<DATA_TYPE>(result) : result;
    }

    [[nodiscard]] std::string ReadString();
    [[nodiscard]] std::string ReadLongString();

    template <typename T, std::size_t ARRAY_SIZE>
    [[nodiscard]] std::array<T, ARRAY_SIZE> ReadArray()
    {
        std::array<T, ARRAY_SIZE> result;
        this->ReadImpl(result);
        return result;
    }

protected:
    [[nodiscard]] inline bool CanReadBytes(
        std::size_t bytesCount) const noexcept
    {
        return this->m_DataView.size_bytes() >=
               this->m_CurDataOffset + bytesCount;
    }

    [[nodiscard]] inline std::size_t GetDataViewSize() const noexcept
    {
        return this->m_DataView.size_bytes();
    }

    void ReadImpl(std::span<std::uint8_t> outData);

private:
    const std::span<const std::uint8_t> m_DataView;
    std::size_t m_CurDataOffset;
};

#endif  // __BUFFERVIEW_H_
