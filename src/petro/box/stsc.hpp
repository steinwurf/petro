// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// sample-to-chunk, partial data-offset information
    /// in non-hint tracks, a sample is an individual frame of video, a
    /// time-contiguous series of video frames, or a time-contiguous compressed
    /// section of audio.
    /// in hint tracks, a sample defines the formation of one or more streaming
    /// packets.
    /// no two samples within a track may share the same time-stamp.
    /// a chunk is a contiguous set of samples for one track.
    class stsc : public full_box
    {
    public:

        struct entry_type
        {
            /// an integer that gives the index of the first chunk in this run
            /// of chunks that share the same samples_per_chunk and
            /// sample_description_index; the index of the first chunk in a
            /// track has the value 1 (the first_chunk field in the first record
            /// of this box has the value 1, identifying that the first sample
            /// maps to the first chunk).
            uint32_t first_chunk;

            /// an integer that gives the number of sample in each of these
            /// chunks.
            uint32_t samples_per_chunk;

            /// an integer that gives the index of the sample entry that
            /// describes the samples in this chunk. The index ranges from 1
            /// to the number of sample entries in the stsd.
            uint32_t sample_description_index;
        };

    public:

        static const std::string TYPE;

    public:
        stsc():
            full_box(stsc::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(size, bs, parent);
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(entry_type{
                    bs.read_uint32_t(),
                    bs.read_uint32_t(),
                    bs.read_uint32_t()});

                m_remaining_bytes -= 4 * 3;
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (";
            ss << "first_chunk, ";
            ss << "samples_per_chunk, ";
            ss << "sample_description_index): ";
            auto seperator = "";
            uint32_t max_print = 5;
            for (uint32_t i = 0; i < std::min(
                (uint32_t)m_entries.size(), max_print); ++i)
            {
                auto entry = m_entries[i];
                ss << seperator;
                ss << "("
                    << entry.first_chunk << ","
                    << entry.samples_per_chunk << ","
                    << entry.sample_description_index << ")";
                seperator =  ", ";
            }
            if (m_entries.size() > max_print)
                ss << "...";
            ss << std::endl;
            return ss.str();
        }

        const std::vector<entry_type>& entries() const
        {
            return m_entries;
        }

        uint32_t entry_count() const
        {
            return m_entry_count;
        }

        uint32_t samples_for_chunk(uint32_t chunk_index) const
        {
            for (uint32_t i = entry_count() - 1; i > 0; --i)
            {
                const auto& entry = m_entries[i];
                // std::cout << entry.first_chunk << " <= " << chunk_index + 1<< std::endl;
                if (entry.first_chunk <= (chunk_index + 1))
                {
                    return entry.samples_per_chunk;
                }
            }
            // assert(0);
        }

    private:

        /// an integer that gives the number of entries in the following table
        uint32_t m_entry_count;

        /// vector containing entries
        std::vector<entry_type> m_entries;
    };

    const std::string stsc::TYPE = "stsc";
}
}
