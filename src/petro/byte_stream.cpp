// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "byte_stream.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <vector>

namespace petro
{
    byte_stream::byte_stream(const uint8_t* data, uint64_t size):
        m_data(data),
        m_remaining_bytes(size)
    { }

    byte_stream::byte_stream(byte_stream& bs, uint64_t size):
        m_data(bs.m_data),
        m_remaining_bytes(size),
        m_offset(bs.m_offset)
    {
        bs.m_remaining_bytes -= size;
        bs.m_offset += size;
    }

    void byte_stream::skip(uint64_t bytes)
    {
        assert(m_remaining_bytes >= bytes);
        m_offset += bytes;
        m_remaining_bytes -= bytes;
    }

    uint8_t byte_stream::read_uint8_t()
    {
        assert(m_remaining_bytes >= 1);
        uint8_t result = m_data[m_offset++];
        m_remaining_bytes -= 1;
        return result;
    }

    int16_t byte_stream::read_int16_t()
    {
        assert(m_remaining_bytes >= 2);

        return
            (int16_t) read_uint8_t() << 8 |
            (int16_t) read_uint8_t();
    }

    uint16_t byte_stream::read_uint16_t()
    {
        assert(m_remaining_bytes >= 2);

        return
            (uint16_t) read_uint8_t() << 8 |
            (uint16_t) read_uint8_t();
    }

    int32_t byte_stream::read_int32_t()
    {
        assert(m_remaining_bytes >= 4);

        return
           (int32_t) read_uint8_t() << 24 |
           (int32_t) read_uint8_t() << 16 |
           (int32_t) read_uint8_t() << 8 |
           (int32_t) read_uint8_t();
    }

    uint32_t byte_stream::read_uint32_t()
    {
        assert(m_remaining_bytes >= 4);

        return
           (uint32_t) read_uint8_t() << 24 |
           (uint32_t) read_uint8_t() << 16 |
           (uint32_t) read_uint8_t() << 8 |
           (uint32_t) read_uint8_t();
    }

    int64_t byte_stream::read_int64_t()
    {
        assert(m_remaining_bytes >= 8);

        return
           (int64_t) read_uint8_t() << 56 |
           (int64_t) read_uint8_t() << 48 |
           (int64_t) read_uint8_t() << 40 |
           (int64_t) read_uint8_t() << 32 |
           (int64_t) read_uint8_t() << 24 |
           (int64_t) read_uint8_t() << 16 |
           (int64_t) read_uint8_t() << 8 |
           (int64_t) read_uint8_t();
    }

    uint64_t byte_stream::read_uint64_t()
    {
        assert(m_remaining_bytes >= 8);

        return
           (uint64_t) read_uint8_t() << 56 |
           (uint64_t) read_uint8_t() << 48 |
           (uint64_t) read_uint8_t() << 40 |
           (uint64_t) read_uint8_t() << 32 |
           (uint64_t) read_uint8_t() << 24 |
           (uint64_t) read_uint8_t() << 16 |
           (uint64_t) read_uint8_t() << 8 |
           (uint64_t) read_uint8_t();
    }

    std::string byte_stream::read_type()
    {
        assert(m_remaining_bytes >= 4);
        char c1 = read_uint8_t();
        char c2 = read_uint8_t();
        char c3 = read_uint8_t();
        char c4 = read_uint8_t();

        return {c1, c2, c3, c4};
    }

    double byte_stream::read_fixed_point_1616()
    {
        return ((double) read_uint32_t()) / (1 << 16);
    }


    double byte_stream::read_fixed_point_0230()
    {
        return ((double) read_uint32_t()) / (1 << 30);
    }

    float byte_stream::read_fixed_point_88()
    {
        return ((float) read_uint16_t()) / (1 << 8);
    }

    std::string byte_stream::read_iso639()
    {
        uint16_t n = read_uint16_t();
        char c1 = 0x60 + ((n & 0x7C00) >> 10);  // Mask is 0111 1100 0000 0000
        char c2 = 0x60 + ((n & 0x03E0) >> 5);   // Mask is 0000 0011 1110 0000
        char c3 = 0x60 + ((n & 0x001F));        // Mask is 0000 0000 0001 1111
        return {c1, c2, c3};
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
        // mp4 time  is the seconds since 00:00, Jan 1 1904 UTC
        // time_t    is the seconds since 00:00, Jan 1 1970 UTC
        // seconds between 01/01/1904 00:00:00 and 01/01/1970 00:00:00
        uint64_t seconds_between_1904_and_1970 = 2082844800;

        // handle the limited time representation of time_t.
        if (total_time < seconds_between_1904_and_1970)
        {
            return "before 1970-01-01 00:00:00";
        }

        std::time_t t = total_time - seconds_between_1904_and_1970;
        // Convert time_t to tm as UTC time
        auto utc_time = std::gmtime(&t);
        char buffer[20];
        std::strftime(buffer, 20, "%F %T", utc_time);
        return std::string(buffer);
    }

    uint64_t byte_stream::remaining_bytes() const
    {
        return m_remaining_bytes;
    }

    const uint8_t* byte_stream::data() const
    {
        return m_data;
    }

    const uint8_t* byte_stream::data_offset() const
    {
        return m_data + m_offset;
    }

    uint64_t byte_stream::offset() const
    {
        return m_offset;
    }
}
