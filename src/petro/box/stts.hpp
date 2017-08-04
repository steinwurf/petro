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

public:

    static const std::string TYPE;

public:

    stts(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    // void read(uint32_t size, byte_stream& bs)
    // {
    //     full_box::read(size, bs);
    //     m_entry_count = bs.read_uint32_t();
    //     m_remaining_bytes -= 4;

    //     uint32_t decoding_time = 0;
    //     for (uint32_t i = 0; i < m_entry_count; ++i)
    //     {
    //         uint32_t sample_count = bs.read_uint32_t();
    //         uint32_t sample_delta = bs.read_uint32_t();
    //         m_entries.push_back(entry_type
    //         {
    //             sample_count,
    //             sample_delta
    //         });
    //         m_remaining_bytes -= 8;

    //         for (uint32_t i = 0; i < sample_count; ++i)
    //         {
    //             decoding_time += sample_delta;
    //             m_decoding_times.push_back(decoding_time);
    //         }

    //         for (uint32_t i = 0; i < sample_count; ++i)
    //         {
    //             m_durations.push_back(sample_delta);
    //         }
    //     }
    //     assert(m_decoding_times.size() == m_durations.size());
    //     bs.skip(m_remaining_bytes);
    // }

    // virtual std::string describe() const
    // {
    //     std::stringstream ss;
    //     ss << full_box::describe() << std::endl;
    //     ss << "  entry_count: " << m_entry_count << std::endl;
    //     ss << "  entries (count, delta): ";
    //     auto seperator = "";
    //     uint32_t max_print = 5;
    //     for (uint32_t i = 0;
    //          i < std::min((uint32_t)m_entries.size(), max_print); ++i)
    //     {
    //         auto entry = m_entries[i];
    //         ss << seperator;
    //         ss << "(" << entry.sample_count << ", "
    //            << entry.sample_delta << ")";
    //         seperator =  ", ";
    //     }
    //     if (m_entries.size() > max_print)
    //         ss << "...";
    //     ss << std::endl;
    //     return ss.str();
    // }

    uint32_t samples() const
    {
        return m_decoding_times.size();
    }

    uint32_t decoding_time(uint32_t sample_index) const
    {
        assert(sample_index < m_decoding_times.size());
        return m_decoding_times[sample_index];
    }

    uint32_t duration(uint32_t sample_index) const
    {
        assert(sample_index < m_durations.size());
        return m_durations[sample_index];
    }

private:

    /// an integer that counts the actual entries
    uint32_t m_entry_count;

    /// a vector containing each entry's sample count and sample delta.
    std::vector<entry_type> m_entries;

    /// the decoding time for each sample.
    /// this field is not part of the standard, and is simply a result of
    /// the extraction of the data in the entries.
    std::vector<uint32_t> m_decoding_times;

    /// the duration for each sample.
    /// this field is not part of the standard, and is simply a result of
    /// the extraction of the data in the entries.
    std::vector<uint32_t> m_durations;
};
}
}
