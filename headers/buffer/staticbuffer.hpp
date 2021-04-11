#ifndef __BUFFER_STATICBUFFER_H_
#define __BUFFER_STATICBUFFER_H_

#include <array>
#include <cstdint>
#include <cstring>
#include <span>
#include <stdexcept>
#include <string_view>

#include "util/binary.hpp"

template <std::size_t BUFFER_SIZE>
class StaticBuffer
{
public:
    StaticBuffer() : m_CurOffset(0) {}

    template <typename T>
    inline void Write(
        T data, bool bigEndian = false) requires std::is_arithmetic<T>::value
    {
        auto srcData = bigEndian ? ReverseEndianness<T>(data) : data;
        auto srcPtr = reinterpret_cast<std::uint8_t*>(&srcData);
        this->WriteArray(std::span<const std::uint8_t>{ srcPtr, sizeof(T) });
    }

    template <typename T>
    inline void WriteToOffset(
        T data, std::size_t offset,
        bool bigEndian = false) requires std::is_arithmetic<T>::value
    {
        auto srcData = bigEndian ? ReverseEndianness<T>(data) : data;
        auto srcPtr = reinterpret_cast<std::uint8_t*>(&srcData);
        this->WriteToOffsetArray(std::span<const std::uint8_t>{ srcPtr, sizeof(T) },
                                 offset);
    }

    void WriteString(std::string_view str)
    {
        if (str.size() >= std::numeric_limits<std::uint8_t>::max())
        {
            throw std::length_error("String to write is too large");
        }

        auto length = str.size();
        this->Write(std::uint8_t(length));

        auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
        this->WriteArray(std::span<const std::uint8_t>{ srcPtr, str.size() });
    }

    void WriteLongString(std::string_view str)
    {
        if (str.size() >= std::numeric_limits<std::uint16_t>::max())
        {
            throw std::length_error("String to write is too large");
        }

        auto length = str.size();
        this->Write(std::uint16_t(length));

        auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
        this->WriteArray(std::span<const std::uint8_t>{ srcPtr, str.size() });
    }

    void WriteArray(std::span<const std::uint8_t> data)
    {
        if (this->m_CurOffset + data.size_bytes() >= BUFFER_SIZE)
        {
            throw std::length_error("Data does not fit inside the buffer");
        }

        std::memcpy(this->m_Buffer.data() + this->m_CurOffset, data.data(),
                    data.size_bytes());
        this->m_CurOffset += data.size_bytes();
    }

    void WriteToOffsetArray(std::span<const std::uint8_t> data, std::size_t offset)
    {
        if (offset >= this->m_CurOffset)
        {
            throw std::runtime_error("Offset is out of buffer bounds");
        }

        std::memcpy(this->m_Buffer.data() + offset, data.data(),
                    data.size_bytes());
    }

    [[nodiscard]] const std::span<const std::uint8_t> GetDataView() const noexcept
    {
        return std::span<const std::uint8_t>{ this->m_Buffer.data(),
                                         this->m_CurOffset };
    }

    [[nodiscard]] inline std::size_t GetCurOffset() const noexcept
    {
        return this->m_CurOffset;
    }

private:
    std::array<std::uint8_t, BUFFER_SIZE> m_Buffer;
    std::size_t m_CurOffset;
};

#endif  // __BUFFER_STATICBUFFER_H_
