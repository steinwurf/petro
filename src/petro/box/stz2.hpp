// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "full_box.hpp"

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

    void parse_full_box_content(std::error_code& error) override
    {
        // reserved
        m_bs.skip(3, error);
        if (error)
            return;

        m_bs.read(m_field_size, error);
        if (error)
            return;

        m_bs.read(m_sample_count, error);
        if (error)
            return;

        for (uint32_t i = 0; i < m_sample_count; ++i)
        {
            if (m_field_size == 4)
            {
                uint8_t data_value = 0;
                m_bs.read(data_value, error);
                if (error)
                    return;

                m_entry_sizes.push_back(data_value & 0x0F);
                m_entry_sizes.push_back(data_value & 0xF0);
                i += 1;
            }
            else if (m_field_size == 8)
            {
                uint8_t data_value = 0;
                m_bs.read(data_value, error);
                if (error)
                    return;
                m_entry_sizes.push_back(data_value);
            }
            else if (m_field_size == 16)
            {
                uint16_t data_value;
                m_bs.read(data_value, error);
                if (error)
                    return;
                m_entry_sizes.push_back(data_value);
            }
            else
            {
                // illegal field size
                error = box_error_code();
                return;
            }
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
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
