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
    /// (composition) time to sample
    class ctts : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        ctts(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(ctts::TYPE, size, bs, parent)
        {
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(std::pair<uint32_t, uint32_t>(
                    bs.read_uint32_t(),
                    bs.read_uint32_t()));
                m_remaining_bytes -= 8;
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (count, offset): ";
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

        /// a vector containing each entry's sample count and sample offset.
        /// The sample count is an integer that counts the number of consecutive
        /// samples that have the given offset.
        /// The sample offset is a non-negative integer that gives the offset
        /// between CT and DT, such that CT(n) = DT(n) + CTTS(n).
        std::vector<std::pair<uint32_t,uint32_t>> m_entries;
    };

    const std::string ctts::TYPE = "ctts";
}
}
