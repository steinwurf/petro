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

        using entry_type = std::pair<uint32_t,uint32_t>;

    public:

        static const std::string TYPE;

    public:
        stts():
            full_box(stts::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(size, bs, parent);
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(
                    entry_type(bs.read_uint32_t(), bs.read_uint32_t()));
                m_remaining_bytes -= 8;
            }
            bs.skip(m_remaining_bytes);
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
                ss << "(" << entry.first << ", " << entry.second << ")";
                seperator =  ", ";
            }
            ss << std::endl;

            return ss.str();
        }

        const std::vector<entry_type>& entries() const
        {
            return m_entries;
        }

    private:

        /// an integer that counts the actual entries
        uint32_t m_entry_count;

        /// a vector containing each entry's sample count and sample delta.
        /// The sample count is an integer that counts the number of consecutive
        /// samples that have the given duration.
        /// The sample delta is an integer that gives the delta of these samples
        /// in the time-scale of the media.
        std::vector<entry_type> m_entries;
    };

    const std::string stts::TYPE = "stts";
}
}
