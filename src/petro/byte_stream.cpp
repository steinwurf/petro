// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "byte_stream.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <ctime>


namespace petro
{
    byte_stream::byte_stream(const uint8_t* data, uint32_t size):
        m_data(data),
        m_size(size)
    { }

    void byte_stream::skip(uint32_t bytes)
    {
        assert(m_size >= bytes);
        m_data += bytes;
        m_size -= bytes;
    }

    uint8_t byte_stream::read_uint8_t()
    {
        assert(m_size >= 1);
        uint8_t result = m_data[0];
        m_data += 1;
        m_size -= 1;
        return result;
    }

    int16_t byte_stream::read_int16_t()
    {
        assert(m_size >= 2);
        int16_t result = ((int16_t*) m_data)[0];
        m_data += 2;
        m_size -= 2;
        return result;
    }

    uint16_t byte_stream::read_uint16_t()
    {
        assert(m_size >= 2);
        uint16_t result = ((uint16_t) m_data[0] << 8 | (uint16_t)m_data[1]);
        m_data += 2;
        m_size -= 2;
        return result;
    }

    int32_t byte_stream::read_int32_t()
    {
        assert(m_size >= 4);

        int32_t result = ((int32_t*) m_data)[0];

        m_data += 4;
        m_size -= 4;
        return result;
    }

    uint32_t byte_stream::read_uint32_t()
    {
        assert(m_size >= 4);

        uint32_t result =
           (uint32_t) m_data[0] << 24 |
           (uint32_t) m_data[1] << 16 |
           (uint32_t) m_data[2] << 8 |
           (uint32_t) m_data[3];
        m_data += 4;
        m_size -= 4;
        return result;
    }

    int64_t byte_stream::read_int64_t()
    {
        assert(m_size >= 8);
        int64_t result = ((int64_t*) m_data)[0];
        m_data += 8;
        m_size -= 8;
        return result;
    }

    uint64_t byte_stream::read_uint64_t()
    {
        assert(m_size >= 8);
        uint64_t result =
           (uint64_t) m_data[0] << 56 |
           (uint64_t) m_data[1] << 48 |
           (uint64_t) m_data[2] << 40 |
           (uint64_t) m_data[3] << 32 |
           (uint64_t) m_data[4] << 24 |
           (uint64_t) m_data[5] << 16 |
           (uint64_t) m_data[6] << 8 |
           (uint64_t) m_data[7];
        m_data += 8;
        m_size -= 8;
        return result;
    }

    std::string byte_stream::read_type()
    {
        assert(m_size >= 4);

        std::string result;

        result += read_uint8_t();
        result += read_uint8_t();
        result += read_uint8_t();
        result += read_uint8_t();

        return result;
    }

    float byte_stream::read_fixed_point(
        uint32_t integer_length, uint32_t fractional_length)
    {
        uint32_t n;
        if ((integer_length + fractional_length) == 16)
        {
            n = read_uint16_t();
        }
        else
        {
            n = read_uint32_t();
        }

        uint32_t integer = n >> fractional_length;
        uint32_t fractional_mask = std::pow(2, fractional_length) - 1;
        uint32_t fractional = (n & fractional_mask) / (1 << fractional_length);

        return integer + fractional;
    }

    std::string byte_stream::read_iso639_code()
    {
        uint16_t n = read_uint16_t();

        uint8_t c1 = (n & 0x7C00) >> 10;  // Mask is 0111 1100 0000 0000
        uint8_t c2 = (n & 0x03E0) >> 5;   // Mask is 0000 0011 1110 0000
        uint8_t c3 = (n & 0x001F);        // Mask is 0000 0000 0001 1111

        c1 += 0x60;
        c2 += 0x60;
        c3 += 0x60;

        std::string result;
        result.append((char*)&c1, 1);
        result.append((char*)&c2, 1);
        result.append((char*)&c3, 1);

        return result;
    }


    std::string byte_stream::read_time32()
    {
        return read_time(read_uint32_t());
    }

    std::string byte_stream::read_time64()
    {
        return read_time(read_uint64_t());
    }

    std::string byte_stream::read_time(uint64_t total_time)
    {
        // 2082844800 seconds between 01/01/1904 & 01/01/1970
        // 2081376000 + 1468800 (66 years + 17 leap days)
        total_time -= 2082844800;

        // asctime creates a char* with the following format:
        //     Www Mmm dd hh:mm:ss yyyy\n
        std::time_t t = total_time;
        char* time_chars = std::asctime(std::localtime(&t));

        std::stringstream ss;
        // We don't want the trailing newline so we only pick the first 24
        // chars.
        ss << std::string(time_chars, 24);
        return ss.str();
    }

    uint32_t byte_stream::size() const
    {
        return m_size;
    }

    const uint8_t* byte_stream::data() const
    {
        return m_data;
    }
}
