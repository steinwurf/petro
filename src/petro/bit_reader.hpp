// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

namespace petro
{
    /// Bit reader for reading packed bits.
    /// The reading is done in most significant byte first, most significant
    /// bit first order. Example:
    ///
    /// 5 bits: 2 (00010)
    /// 4 bits: 4 (0100)
    /// 4 bits: 2 (0010)
    /// 3 bits: 0 (000)
    ///
    /// Byte 1: 00010010
    /// Byte 2: 00010000
    ///
    /// 00010010 00010000
    /// [ 2 ][ 4 ][2 ][0]
    ///
    class bit_reader
    {

    public:

        bit_reader(uint8_t* data, uint64_t bits):
            m_data(data),
            m_bits(bits),
            m_bit_offset(0)
        { }

        bit_reader(std::vector<uint8_t>& data):
            m_data(data.data()),
            m_bits(data.size() * 8),
            m_bit_offset(0)
        { }

        uint8_t read_next_bit()
        {
            assert(m_bit_offset < m_bits);
            auto byte = m_bit_offset / 8;
            auto position = 7 - (m_bit_offset % 8);
            m_bit_offset += 1;
            return (m_data[byte] >> position) & 0x1;
        }

        void skip(uint64_t bits)
        {
            m_bit_offset += bits;
            assert(m_bit_offset < m_bits);
        }

        uint8_t read_1_bit()
        {
            return read_next_bit();
        }

        uint8_t read_2_bits()
        {
            return
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_3_bits()
        {
            return
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_4_bits()
        {
            return
                (read_next_bit() << 3) |
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_5_bits()
        {
            return
                (read_next_bit() << 4) |
                (read_next_bit() << 3) |
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_6_bits()
        {
            return
                (read_next_bit() << 5) |
                (read_next_bit() << 4) |
                (read_next_bit() << 3) |
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_7_bits()
        {
            return
                (read_next_bit() << 6) |
                (read_next_bit() << 5) |
                (read_next_bit() << 4) |
                (read_next_bit() << 3) |
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }
        uint8_t read_8_bits()
        {
            return
                (read_next_bit() << 7) |
                (read_next_bit() << 6) |
                (read_next_bit() << 5) |
                (read_next_bit() << 4) |
                (read_next_bit() << 3) |
                (read_next_bit() << 2) |
                (read_next_bit() << 1) |
                (read_next_bit() << 0);
        }

        uint64_t size() const
        {
            return m_bits;
        }


    private:

        uint8_t* m_data;
        uint64_t m_bits;
        uint64_t m_bit_offset;

    };
}
