// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <ctime>

#include <endian/big_endian.hpp>
#include <endian/stream_reader.hpp>

#include "error.hpp"
#include "matrix.hpp"
#include "descriptor/tag.hpp"

namespace petro
{
class stream : private endian::stream_reader<endian::big_endian>
{
public:
    // Expose some of the
    using endian::stream_reader<endian::big_endian>::position;
    using endian::stream_reader<endian::big_endian>::data;
    using endian::stream_reader<endian::big_endian>::size;
    using endian::stream_reader<endian::big_endian>::remaining_size;
    using endian::stream_reader<endian::big_endian>::remaining_data;

public:

    stream(const uint8_t* data, uint64_t size) :
        endian::stream_reader<endian::big_endian>(data, size)
    { }

    template<class ValueType>
    void read(ValueType& value, std::error_code& error)
    {
        assert(!error);
        // Make sure there is enough data to read in the underlying buffer
        if (sizeof(ValueType) > remaining_size())
        {
            error = m_error;
            return;
        }
        endian::stream_reader<endian::big_endian>::read(value);
    }

    void read(std::string& string, uint64_t size, std::error_code& error)
    {
        string.resize(size);
        read((uint8_t*)string.data(), size, error);
    }

    void read(uint8_t* data, uint64_t size, std::error_code& error)
    {
        assert(!error);
        // Make sure there is enough data to read in the underlying buffer
        if (size > remaining_size())
        {
            error = m_error;
            return;
        }
        endian::stream_reader<endian::big_endian>::read(data, size);
    }

    void seek(uint64_t new_position, std::error_code& error)
    {
        assert(!error);
        if (new_position > size())
        {
            error = m_error;
            return;
        }
        endian::stream_reader<endian::big_endian>::seek(new_position);
    }

    void skip(uint64_t bytes_to_skip, std::error_code& error)
    {
        assert(!error);
        if (bytes_to_skip > remaining_size())
        {
            error = m_error;
            return;
        }
        endian::stream_reader<endian::big_endian>::skip(bytes_to_skip);
    }

    void set_error_code(petro::error error)
    {
        m_error = error;
    }

public:

    void read_type(std::string& type, std::error_code& error)
    {
        read(type, 4, error);
    }

    void read_fixed_point_1616(double& value, std::error_code& error)
    {
        uint32_t data = 0;
        read(data, error);
        if (error)
            return;
        value = ((double) data) / (1 << 16);
    }
    void read_fixed_point_0230(double& value, std::error_code& error)
    {
        uint32_t data = 0;
        read(data, error);
        if (error)
            return;
        value = ((double) data) / (1 << 30);
    }

    void read_fixed_point_88(float& value, std::error_code& error)
    {
        uint16_t data = 0;
        read(data, error);
        if (error)
            return;
        value = ((float) data) / (1 << 8);
    }

    void read_iso639(std::string& value, std::error_code& error)
    {
        uint16_t data = 0;
        read(data, error);
        if (error)
            return;

        char c1 = 0x60 + ((data & 0x7C00) >> 10);  // Mask is 0111 1100 0000 0000
        char c2 = 0x60 + ((data & 0x03E0) >> 5);   // Mask is 0000 0011 1110 0000
        char c3 = 0x60 + ((data & 0x001F));        // Mask is 0000 0000 0001 1111
        value = {c1, c2, c3};
    }

    void read_time32(std::string& value, std::error_code& error)
    {
        uint32_t data = 0;
        read(data, error);
        if (error)
            return;
        value = time(data);
    }

    void read_time64(std::string& value, std::error_code& error)
    {
        uint64_t data = 0;
        read(data, error);
        if (error)
            return;
        value = time(data);
    }

    void read(matrix& matrix, std::error_code& error)
    {
        read_fixed_point_1616(matrix.a, error);
        if (error)
            return;

        read_fixed_point_1616(matrix.b, error);
        if (error)
            return;

        read_fixed_point_0230(matrix.u, error);
        if (error)
            return;

        read_fixed_point_1616(matrix.c, error);
        if (error)
            return;

        read_fixed_point_1616(matrix.d, error);
        if (error)
            return;

        read_fixed_point_0230(matrix.v, error);
        if (error)
            return;

        read_fixed_point_1616(matrix.x, error);
        if (error)
            return;

        read_fixed_point_1616(matrix.y, error);
        if (error)
            return;

        read_fixed_point_0230(matrix.w, error);
        if (error)
            return;
    }

    void read(descriptor::tag& tag, std::error_code& error)
    {
        uint8_t tag_value = 0;
        read(tag_value, error);
        if (error)
            return;

        tag = descriptor::tag(tag_value);
    }

private:

    std::string time(uint64_t data)
    {
        // mp4 time  is the seconds since 00:00, Jan 1 1904 UTC
        // time_t    is the seconds since 00:00, Jan 1 1970 UTC
        // seconds between 01/01/1904 00:00:00 and 01/01/1970 00:00:00
        uint64_t seconds_between_1904_and_1970 = 2082844800;

        // handle the limited time representation of time_t.
        if (data < seconds_between_1904_and_1970)
        {
            return "before 1970-01-01 00:00:00";
        }

        std::time_t t = data - seconds_between_1904_and_1970;
        // Convert time_t to tm as UTC time
        auto utc_time = std::gmtime(&t);
        char buffer[20];
        std::strftime(buffer, 20, "%F %T", utc_time);
        return std::string(buffer);
    }

private:

    petro::error m_error = petro::error::invalid_stream_read;
};
}
