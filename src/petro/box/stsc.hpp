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
    class stsc : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        stsc(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(stsc::TYPE, size, bs, parent)
        {
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(std::tuple<uint32_t,uint32_t,uint32_t>(
                    bs.read_uint32_t(),
                    bs.read_uint32_t(),
                    bs.read_uint32_t()
                    ));
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
                    << std::get<0>(entry) << ","
                    << std::get<1>(entry) << ","
                    << std::get<2>(entry) << ")";
                seperator =  ", ";
            }
            if (m_entries.size() > max_print)
                ss << "...";
            ss << std::endl;
            return ss.str();
        }

    private:

        /// an integer that gives the number of entries in the following table
        uint32_t m_entry_count;

        /// vector containing entires of
        /// (first_chunk, samples_per_chunk, sample_description_index)
        /// where;
        /// first_chunk is an integer that gives the index of the  first chunk
        /// in this run of chunks that share the same samples-per-chunk and
        /// sample-description-index; the index of the first chunk in a track
        /// has the value 1 (the first_chunk field in the first record of this
        /// box has the value 1, identifying that the first sample maps to
        /// the first chunk).
        /// samples_per_chunk is an integer that gives the number of samples in
        /// each of these chunks.
        /// sample_description_index is an integer that gives the index of the
        /// sample entry that describes the samples in this chunk. The index
        /// ranges from 1 to the number of sample entries in the stsd.
        std::vector<std::tuple<uint32_t,uint32_t,uint32_t>> m_entries;
    };

    const std::string stsc::TYPE = "stsc";
}
}
