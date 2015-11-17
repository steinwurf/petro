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
    /// (decoding) time-to-sample
    class stts : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        stts(byte_stream& bs, uint32_t size):
            full_box(stts::TYPE, bs, size)
        {
            m_entry_count = bs.read_uint32_t();
            size -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(std::pair<uint32_t, uint32_t>(
                    bs.read_uint32_t(),
                    bs.read_uint32_t()));
                size -= 8;
            }
            bs.skip(size);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (count, delta): ";
            auto seperator = "";
            for (const auto& entry : m_entries)
            {
                ss << seperator;
                ss << "(" << entry.first << ", " << entry.first << ")";
                seperator =  ", ";
            }
            ss << std::endl;

            return ss.str();
        }

        /// an integer that counts the actual entries
        uint32_t m_entry_count;

        /// a vector containing each entry's sample count and sample delta.
        /// The sample count is an integer that counts the number of consecutive
        /// samples that have the given duration.
        /// The sample delta is an integer that gives the delta of these samples
        /// in the time-scale of the media.
        std::vector<std::pair<uint32_t,uint32_t>> m_entries;
    };

    const std::string stts::TYPE = "stts";
}
}
