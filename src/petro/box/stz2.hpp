// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// compact sample sizes (framing)
class stz2 : public full_box
{

public:

    static const std::string TYPE;

public:
    stz2(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
        bs.skip(3);
        m_remaining_bytes -= 3;

        m_field_size = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        m_sample_count = bs.read_uint32_t();
        m_remaining_bytes -= 4;

        for (uint32_t i = 0; i < m_sample_count; ++i)
        {
            if (m_field_size == 4)
            {
                uint8_t data = bs.read_uint8_t();
                m_entry_sizes.push_back(data & 0x0F);
                m_entry_sizes.push_back(data & 0xF0);
                m_remaining_bytes -= 1;
                i += 1;
            }
            else if (m_field_size == 8)
            {
                m_entry_sizes.push_back(bs.read_uint8_t());
                m_remaining_bytes -= 1;
            }
            else if (m_field_size == 16)
            {
                m_entry_sizes.push_back(bs.read_uint16_t());
                m_remaining_bytes -= 2;
            }
            else
            {
                // illegal field size
                assert(0);
            }
            m_entry_sizes.push_back(bs.read_uint32_t());
            m_remaining_bytes -= 4;
        }
        bs.skip(m_remaining_bytes);
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        ss << "  field_size: " << m_field_size << std::endl;
        ss << "  sample_count: " << m_sample_count << std::endl;
        ss << "  samples (size): ";
        auto seperator = "";
        uint32_t max_print = 5;
        for (uint32_t i = 0;
             i < std::min((uint32_t)m_entry_sizes.size(), max_print); ++i)
        {
            ss << seperator;
            ss << "(" << m_entry_sizes[i] << ")";
            seperator =  ", ";
        }
        if (m_entry_sizes.size() > max_print)
            ss << "...";
        ss << std::endl;
        return ss.str();
    }

private:
    /// an integer specifying the size in bits of the entries in the
    /// following table; it shall take the value 4, 8 or 16. If the value 4
    /// is used, then each byte contains two values:
    ///     entry[i]<<4 + entry[i+1];
    /// if the sizes do not fill an integral number of bytes, the last byte
    /// is padded with zeros.
    uint8_t m_field_size;

    /// an integer that gives the number of samples in the track; if
    /// m_sample_size is 0, then it is also the number of entries in the
    /// following table.
    uint32_t m_sample_count;

    /// a vector of integers specifying the size of the samples, indexed by
    /// its number.
    std::vector<uint16_t> m_entry_sizes;
};
}
}
