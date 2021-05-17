// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>

#include <endian/big_endian.hpp>
#include <endian/stream_reader.hpp>

#include "descriptor/tag.hpp"
#include "error.hpp"
#include "matrix.hpp"

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
    stream(const uint8_t* data, uint64_t size);

    template <class ValueType>
    void read(ValueType& value, std::error_code& error)
    {
        assert(!error);
        // Make sure there is enough data to read in the underlying buffer
        if (sizeof(ValueType) > remaining_size())
        {
            error = m_error;
            return;
        }
        endian::stream_reader<endian::big_endian>::read<ValueType>(value);
    }

    void read(std::string& string, uint64_t size, std::error_code& error);

    void read(uint8_t* data, uint64_t size, std::error_code& error);

    void seek(uint64_t new_position, std::error_code& error);

    void skip(uint64_t bytes_to_skip, std::error_code& error);

    void set_error_code(petro::error error);

public:
    void read_type(std::string& type, std::error_code& error);

    void read_fixed_point_1616(double& value, std::error_code& error);

    void read_fixed_point_0230(double& value, std::error_code& error);

    void read_fixed_point_88(float& value, std::error_code& error);

    void read_iso639(std::string& value, std::error_code& error);

    void read_time32(std::string& value, std::error_code& error);

    void read_time64(std::string& value, std::error_code& error);

    void read(matrix& matrix, std::error_code& error);

    void read(descriptor::tag& tag, std::error_code& error);

private:
    std::string time(uint64_t data);

private:
    petro::error m_error = petro::error::invalid_stream_read;
};
}
