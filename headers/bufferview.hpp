#ifndef __BUFFERVIEW_H_
#define __BUFFERVIEW_H_

#include <cstdint>
#include <span>
#include <stdexcept>

#include "util/binary.hpp"

class BufferView
{
public:
    BufferView(const std::span<const std::uint8_t> data)
        : m_DataView(data), m_CurDataOffset(0)
    {
    }

    template <typename DATA_TYPE>
    [[nodiscard]] DATA_TYPE Read(
        bool bigEndian = false) requires std::is_arithmetic<DATA_TYPE>::value
    {
        if (this->CanReadBytes(sizeof(DATA_TYPE)) == false)
        {
            throw std::out_of_range("The data buffer is too small");
        }

        auto res = *reinterpret_cast<const DATA_TYPE*>(
            &this->m_DataView[this->m_CurDataOffset]);
        this->m_CurDataOffset += sizeof(DATA_TYPE);

        if (bigEndian == true)
        {
            res = ReverseEndianness<DATA_TYPE>(res);
        }

        return res;
    }

    [[nodiscard]] std::string ReadString()
    {
        std::size_t length = this->Read<std::uint8_t>();

        if (length > this->GetRemainingBytes())
        {
            throw std::length_error(
                "The string's length is larger than the available data");
        }

        std::string newStr;

        if (length > 0)
        {
            auto strStart = reinterpret_cast<const char*>(
                &this->m_DataView[this->m_CurDataOffset]);

            newStr.reserve(length + 1);
            newStr.append(strStart, length);

            this->m_CurDataOffset += length;
        }

        return newStr;
    }

    [[nodiscard]] std::string ReadLongString()
    {
        std::size_t length = this->Read<std::uint16_t>();

        if (length > this->GetRemainingBytes())
        {
            throw std::length_error(
                "The string's length is larger than the available data");
        }

        std::string newStr;

        if (length > 0)
        {
            auto strStart = reinterpret_cast<const char*>(
                &this->m_DataView[this->m_CurDataOffset]);

            newStr.reserve(length + 1);
            newStr.append(strStart, length);

            this->m_CurDataOffset += length;
        }

        return newStr;
    }

    template <typename T, std::size_t ARRAY_SIZE>
    [[nodiscard]] std::array<T, ARRAY_SIZE> ReadArray()
    {
        if (ARRAY_SIZE > this->GetRemainingBytes())
        {
            throw std::length_error(
                "The array's length is larger than the available data");
        }

        std::array<T, ARRAY_SIZE> result;

        auto arrayStart = reinterpret_cast<const T*>(
            &this->m_DataView[this->m_CurDataOffset]);

        std::copy(arrayStart, arrayStart + ARRAY_SIZE, result.begin());
        this->m_CurDataOffset += sizeof(T) * ARRAY_SIZE;

        return result;
    }

protected:
    [[nodiscard]] inline bool CanReadBytes(std::size_t bytesCount) const noexcept
    {
        return this->m_DataView.size_bytes() >=
               this->m_CurDataOffset + bytesCount;
    }

    [[nodiscard]] inline std::size_t GetRemainingBytes() const noexcept
    {
        return this->m_DataView.size_bytes() - this->m_CurDataOffset;
    }

    [[nodiscard]] inline std::size_t GetDataViewSize() const noexcept
    {
        return this->m_DataView.size_bytes();
    }

private:
    const std::span<const std::uint8_t> m_DataView;
    std::size_t m_CurDataOffset;
};

#endif  // __BUFFERVIEW_H_
