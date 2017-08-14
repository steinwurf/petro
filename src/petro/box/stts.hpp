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
/// (decoding) time-to-sample
class stts : public full_box
{

public:

    struct entry_type
    {
        /// an integer that counts the number of consecutive samples that
        /// have the given duration.
        uint32_t sample_count;
        /// an integer that gives the delta of these samples in the
        /// time-scale of the media.
        uint32_t sample_delta;
    };

    struct sample
    {
        uint32_t decoding_time;
        uint32_t duration;
    };

public:

    static const std::string TYPE;

public:

    stts(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_entry_count, error);
        if (error)
            return;

        uint32_t decoding_time = 0;
        for (uint32_t i = 0; i < m_entry_count; ++i)
        {
            uint32_t sample_count = 0;
            m_bs.read(sample_count, error);
            if (error)
                return;

            uint32_t sample_delta = 0;
            m_bs.read(sample_delta, error);
            if (error)
                return;

            m_entries.push_back(entry_type
            {
                sample_count,
                sample_delta
            });

            for (uint32_t i = 0; i < sample_count; ++i)
            {
                decoding_time += sample_delta;
                m_samples.push_back(
                {
                    decoding_time,
                    sample_delta
                });
            }
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_stts_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        ss << "  entry_count: " << m_entry_count << std::endl;
        ss << "  entries (count, delta): ";
        auto seperator = "";
        uint32_t max_print = 5;
        for (uint32_t i = 0;
             i < std::min((uint32_t)m_entries.size(), max_print); ++i)
        {
            auto entry = m_entries[i];
            ss << seperator;
            ss << "(" << entry.sample_count << ", "
               << entry.sample_delta << ")";
            seperator =  ", ";
        }
        if (m_entries.size() > max_print)
            ss << "...";
        ss << std::endl;
        return ss.str();
    }

    uint32_t samples() const
    {
        return m_samples.size();
    }

    uint32_t decoding_time(uint32_t sample_index) const
    {
        assert(sample_index < samples());
        return m_samples[sample_index].decoding_time;
    }

    uint32_t duration(uint32_t sample_index) const
    {
        assert(sample_index < samples());
        return m_samples[sample_index].duration;
    }

private:

    /// an integer that counts the actual entries
    uint32_t m_entry_count;

    /// a vector containing each entry's sample count and sample delta.
    std::vector<entry_type> m_entries;

    /// the decoding time and duration for each sample.
    /// this field is not part of the standard, and is simply a result of
    /// the extraction of the data in the entries.
    std::vector<sample> m_samples;
};
}
}
