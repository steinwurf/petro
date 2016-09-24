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
    stsz(std::weak_ptr<box> parent) :
        full_box(stsz::TYPE, parent)
    { }

    void read(uint64_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
        m_sample_size = bs.read_uint32_t();
        m_sample_count = bs.read_uint32_t();
        m_remaining_bytes -= 4 * 2;

        if (m_sample_size == 0)
        {
            for (uint32_t i = 0; i < m_sample_count; ++i)
            {
                m_entries.push_back(bs.read_uint32_t());
                m_remaining_bytes -= 4;
            }
        }

        bs.skip(m_remaining_bytes);
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        ss << "  sample_size: " << m_sample_size << std::endl;
        ss << "  sample_count: " << m_sample_count << std::endl;
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
