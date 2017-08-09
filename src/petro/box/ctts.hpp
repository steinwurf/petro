// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

namespace petro
{
namespace box
{
/// (composition) time to sample
class ctts : public full_box
{
public:

    struct entry_type
    {
        /// The sample count is an integer that counts the number of
        /// consecutive samples that have the given offset.
        uint32_t sample_count;

        /// The sample offset is a non-negative integer that gives the
        /// offset between composition time (CT) and decoding time (DT),
        /// such that CT(n) = DT(n) + CTTS(n).
        uint32_t sample_offset;
    };

public:

    static const std::string TYPE;

public:

    ctts(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_entry_count, error);
        if (error)
            return;

        for (uint32_t i = 0; i < m_entry_count; ++i)
        {
            uint32_t sample_count = 0;
            uint32_t sample_offset = 0;
            m_bs.read(sample_count, error);
            if (error)
                return;

            m_bs.read(sample_offset, error);
            if (error)
                return;

            m_entries.push_back(entry_type
            {
                sample_count,
                sample_offset
            });

            for (uint32_t i = 0; i < sample_count; ++i)
            {
                m_composition_times.push_back(sample_offset);
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
        ss << "  entry_count: " << m_entry_count << std::endl;
        ss << "  entries (count, offset): ";
        auto seperator = "";
        uint32_t max_print = 5;
        for (uint32_t i = 0;
             i < std::min((uint32_t)m_entries.size(), max_print); ++i)
        {
            auto entry = m_entries[i];
            ss << seperator;
            ss << "(" << entry.sample_count << ", ";
            ss << entry.sample_offset << ")";
            seperator =  ", ";
        }
        if (m_entries.size() > max_print)
            ss << "...";
        ss << std::endl;

        return ss.str();
    }
    uint32_t samples() const
    {
        return m_composition_times.size();
    }

    uint32_t composition_time(uint32_t sample_index) const
    {
        assert(sample_index < m_composition_times.size());
        return m_composition_times[sample_index];
    }

private:
    /// an integer that counts the actual entries
    uint32_t m_entry_count;

    /// a vector containing each entry's sample count and sample offset.
    std::vector<entry_type> m_entries;

    /// the composition time for each sample.
    /// this field is not part of the standard, and is simply a result of
    /// the extraction of the data in the entries.
    std::vector<uint32_t> m_composition_times;
};
}
}
