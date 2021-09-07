// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <system_error>
#include <vector>

#include "error.hpp"

namespace petro
{
/// Bit reader for reading packed bits.
/// The reading is done in most significant byte first, most significant
/// bit first order. Example:
///
/// 5 bits: 4 (00100)
/// 4 bits: 8 (1000)
/// 4 bits: 2 (0010)
/// 3 bits: 0 (000)
///
/// Byte 1: 00100100
/// Byte 2: 00010000
///
/// 00100100 00010000
/// [ 2 ][ 4 ][ 2][0]
///
class bit_stream
{
public:
    /// Constructor with pointer
    bit_stream(const uint8_t* data, uint64_t size) :
        m_data(data), m_size(size), m_bit_offset(0)
    {
        // Make sure we can address all the bits
        assert(m_size < std::numeric_limits<uint64_t>::max() / 8);
    }

    /// Constructor with vector
    bit_stream(const std::vector<uint8_t>& data) :
        bit_stream(data.data(), data.size())
    {
    }

    /// Skips bits in bit_stream
    /// @param bits The number of bits to skip
    /// @param error An error to report. Usually called recursively
    void skip(uint64_t bits, std::error_code& error)
    {
        assert(!error);
        if ((m_bit_offset + bits) > bit_stream::bits())
        {
            error = petro::error::invalid_bit_stream_read;
            return;
        }
        m_bit_offset += bits;
    }
    /// Go to specific position in bit_stream
    /// @param offset The offset of the new position
    /// @param error An error to report. Usually called recursively
    void seek(uint64_t offset, std::error_code& error)
    {
        assert(!error);
        if (offset > bits())
        {
            error = petro::error::invalid_bit_stream_read;
            return;
        }
        m_bit_offset = offset;
    }

    /// Read a bit
    /// @param out A number or value type object
    /// @param error An error to report. Usually called recursively
    template <class ValueType>
    void read_bit(ValueType& out, std::error_code& error)
    {
        static_assert(std::is_integral<ValueType>::value, "integer required");
        static_assert(!std::is_same<ValueType, bool>::value, "bool disallowed");
        assert(!error);
        read_bits(out, 1, error);
    }

    /// Read multiple bits
    /// @param out A number or value type object. At most 32 bits
    /// @param bits Number of bits to read
    /// @param error An error to report. Usually called recursively
    template <class ValueType>
    void read_bits(ValueType& out, uint64_t bits, std::error_code& error)
    {
        static_assert(std::is_integral<ValueType>::value, "integer required");
        static_assert(!std::is_same<ValueType, bool>::value, "bool disallowed");
        assert(sizeof(ValueType) * 8 <= 32);
        assert(!error);
        ValueType result = 0;
        for (uint32_t i = 0; i < bits; i++)
        {
            uint8_t v = 0;
            read_next_bit(v, error);
            if (error)
                return;

            result |= (v << (bits - i - 1));
        }
        out = result;
    }

    /// @param out The value to be read
    /// @param error An error to report. Usually called recursively
    void read_unsigned_exponential_golomb_code(uint32_t& out,
                                               std::error_code& error)
    {
        assert(!error);
        auto offset_before = m_bit_offset;
        uint32_t i = 0;
        for (; i < 32U; ++i)
        {
            uint8_t value = 0;
            read_bit(value, error);
            if (error)
            {
                m_bit_offset = offset_before;
                return;
            }

            if (value)
                break;
        }

        uint32_t result = 0;
        read_bits(result, i, error);
        if (error)
        {
            m_bit_offset = offset_before;
            return;
        }
        result += (1 << i) - 1;
        out = result;
    }

    /// @param out The value to be read
    /// @param error An error to report. Usually called recursively
    void read_signed_exponential_golomb_code(int32_t& out,
                                             std::error_code& error)
    {
        assert(!error);
        uint32_t v = 0;
        read_unsigned_exponential_golomb_code(v, error);
        if (error)
            return;

        if (v & 0x01)
        {
            out = (v + 1) / 2;
        }
        else
        {
            out = -(int32_t)(v / 2);
        }
    }

    /// @return The data pointer being read
    const uint8_t* data() const
    {
        return m_data;
    }

    /// @return The size of the data
    uint64_t size() const
    {
        return m_size;
    }

    /// @return The number of bits being read
    uint64_t bits() const
    {
        return size() * 8U;
    }

private:
    /// @param value The byte to be read
    /// @param error An error to report. Usually called recursively
    void read_next_bit(uint8_t& value, std::error_code& error)
    {
        assert(!error);
        if (m_bit_offset >= bits())
        {
            error = petro::error::invalid_bit_stream_read;
            return;
        }

        auto byte = m_bit_offset / 8;
        auto position = 7 - (m_bit_offset % 8);
        m_bit_offset += 1;
        value = (m_data[byte] >> position) & 0x1;
    }

private:
    /// The data to read from
    const uint8_t* m_data;

    /// Size of the data
    const uint64_t m_size;

    /// Current position in the bit_stream
    uint64_t m_bit_offset;
};
}
