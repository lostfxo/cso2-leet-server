#include "buffer/staticbuffer.hpp"

/*#include <cstring>
#include <stdexcept>*/

/*template <std::size_t BUFFER_SIZE>
StaticBuffer<BUFFER_SIZE>::StaticBuffer() : m_CurOffset(0)
{
}

template <std::size_t BUFFER_SIZE>
void StaticBuffer<BUFFER_SIZE>::WriteString(std::string_view str)
{
    if (str.size() >= std::numeric_limits<std::uint8_t>::max())
    {
        throw std::length_error("String to write is too large");
    }

    auto length = str.size();
    this->Write(std::uint8_t(length));

    auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
    this->WriteImpl(std::span<const std::uint8_t>{ srcPtr, str.size() });
}

template <std::size_t BUFFER_SIZE>
void StaticBuffer<BUFFER_SIZE>::WriteLongString(std::string_view str)
{
    if (str.size() >= std::numeric_limits<std::uint16_t>::max())
    {
        throw std::length_error("String to write is too large");
    }

    auto length = str.size();
    this->Write(std::uint16_t(length));

    auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
    this->WriteImpl(std::span<const std::uint8_t>{ srcPtr, str.size() });
}

template <std::size_t BUFFER_SIZE>
void StaticBuffer<BUFFER_SIZE>::WriteArray(std::span<const std::uint8_t> data)
{
    if (this->m_CurOffset >= BUFFER_SIZE)
    {
        throw std::length_error("Data does not fit inside the buffer");
    }

    this->m_Buffer.insert(this->m_Buffer.end(), data.begin(), data.end());
    this->m_CurOffset += data.size_bytes();
}

template <std::size_t BUFFER_SIZE>
void StaticBuffer<BUFFER_SIZE>::WriteToOffsetArray(
    std::span<const std::uint8_t> data, std::size_t offset)
{
    if (offset > m_CurOffset)
    {
        throw std::runtime_error("Offset is out of buffer bounds");
    }

    std::memcpy(this->m_Buffer.data(), data.data(), data.size_bytes());
}

template <std::size_t BUFFER_SIZE>
[[nodiscard]] const std::span<const std::uint8_t>
StaticBuffer<BUFFER_SIZE>::GetDataView() const
{
    return std::span<const std::uint8_t>{ this->m_Buffer.data(), this->m_CurOffset };
}
*/
