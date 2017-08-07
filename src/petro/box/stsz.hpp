// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <memory>
#include <algorithm>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// sample sizes (framing)
class stsz : public full_box
{

public:

    static const std::string TYPE;

public:

    stsz(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_sample_size, error);
        if (error)
            return;

        m_bs.read(m_sample_count, error);
        if (error)
            return;

        if (m_sample_size == 0)
        {
            for (uint32_t i = 0; i < m_sample_count; ++i)
            {
                uint32_t entry_value;
                m_bs.read(entry_value, error);
                if (error)
                    return;
                m_entries.push_back(entry_value);
            }
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        ss << "  sample_size: " << m_sample_size << std::endl;
        ss << "  sample_count: " << m_sample_count << std::endl;
        if (m_sample_size == 0)
        {
            ss << "  entities (size):";
            auto seperator = "";
            uint32_t max_print = 5;
            for (uint32_t i = 0;
                 i < std::min((uint32_t)m_entries.size(), max_print); ++i)
            {
                ss << seperator;
                ss << "(" << m_entries[i] << ")";
                seperator =  ", ";
            }
            if (m_entries.size() > max_print)
                ss << "...";
            ss << std::endl;
        }
        return ss.str();
    }

    uint32_t sample_size(uint32_t sample_index) const
    {
        assert(m_sample_size == 0 || sample_index < m_sample_count);
        if (m_sample_size != 0)
            return m_sample_size;
        else
            return m_entries[sample_index];
    }

    uint32_t sample_count() const
    {
        return m_sample_count;
    }

    const std::vector<uint32_t>& entries() const
    {
        return m_entries;
    }

private:
    /// specifying the default sample size. If all the samples are the same
    /// size, this field contains that size value. If this field is set
    /// to 0, then the samples have different sizes, and those sizes are
    /// stored in the sample size table. If this field is not 0, it
    /// specifies the constant sample size, and no array follows.
    uint32_t m_sample_size;

    /// an integer that gives the number of samples in the track; if
    /// m_sample_size is 0, then it is also the number of entries in the
    /// following table.
    uint32_t m_sample_count;
    /// a vector of integers specifying the size of the samples, indexed by
    /// its number.
    std::vector<uint32_t> m_entries;
};
}
}
