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
    /// sync sample table (random access points)
    class stss : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        stss(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(stss::TYPE, size, bs, parent)
        {
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(bs.read_uint32_t());
                m_remaining_bytes -= 4;
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (sample_number): ";
            auto seperator = "";
            uint32_t max_print = 5;
            for (uint32_t i = 0; i < std::min(
                (uint32_t)m_entries.size(), max_print); ++i)
            {
                auto entry = m_entries[i];
                ss << seperator;
                ss << "(" << entry << ")";
                seperator =  ", ";
            }
            if (m_entries.size() > max_print)
                ss << "...";
            ss << std::endl;
            return ss.str();
        }

    private:

        /// an integer that gives the number of entries in the following table.
        /// If entry_count is zero, there are no random access points within
        /// the stream and the following table is empty.
        uint32_t m_entry_count;

        /// gives the numbers of the samples that are random access points in
        /// the stream.
        std::vector<uint32_t> m_entries;
    };

    const std::string stss::TYPE = "stss";
}
}
