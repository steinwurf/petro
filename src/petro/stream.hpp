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
/// Wrapper class for a stream of data.
class stream : private endian::stream_reader<endian::big_endian>
{
public:
    // Expose some of the Endian API
    using endian::stream_reader<endian::big_endian>::position;
    using endian::stream_reader<endian::big_endian>::data;
    using endian::stream_reader<endian::big_endian>::size;
    using endian::stream_reader<endian::big_endian>::remaining_size;
    using endian::stream_reader<endian::big_endian>::remaining_data;

public:
    /// Constructor
    stream(const uint8_t* data, uint64_t size);

    /// Read the value
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

    /// Read a string with given size
    /// @param string A string of data
    /// @param size The size of the string
    /// @param error An error to report. Usually called recursively
    void read(std::string& string, uint64_t size, std::error_code& error);

    /// Read a data pointer with given size
    /// @param data The memory to read
    /// @param size The size of the string
    /// @param error An error to report. Usually called recursively
    void read(uint8_t* data, uint64_t size, std::error_code& error);

    /// Change the read position in the stream
    /// @param new_position The new position to read from
    /// @param error An error to report. Usually called recursively
    void seek(uint64_t new_position, std::error_code& error);

    /// Skip bytes in the stream
    /// @param bytes_to_skip The number of bytes to skip
    /// @param error An error to report. Usually called recursively
    void skip(uint64_t bytes_to_skip, std::error_code& error);

    /// Sets the error code
    /// @param error An error to report.
    void set_error_code(petro::error error);

public:
    /// Reads the type
    /// @param type string containing the type
    /// @param error An error to report. Usually called recursively
    void read_type(std::string& type, std::error_code& error);

    /// different util reads
    void read_fixed_point_1616(double& value, std::error_code& error);

    /// different util reads
    void read_fixed_point_0230(double& value, std::error_code& error);

    /// different util reads
    void read_fixed_point_88(float& value, std::error_code& error);

    /// different util reads
    void read_iso639(std::string& value, std::error_code& error);

    /// different util reads
    void read_time32(std::string& value, std::error_code& error);

    /// different util reads
    void read_time64(std::string& value, std::error_code& error);

    /// different util reads
    void read(matrix& matrix, std::error_code& error);

    /// different util reads
    void read(descriptor::tag& tag, std::error_code& error);

private:
    /// @return a string containing the time as UTC time
    /// @param data The time as a number in seconds
    std::string time(uint64_t data);

private:
    /// invalid_stream_read error
    petro::error m_error = petro::error::invalid_stream_read;
};
}
