// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <memory>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// chunk offset, partial data-offset information
    class stco : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        stco(std::weak_ptr<box> parent):
            full_box(stco::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            full_box::read(size, bs);
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
            ss << "  entries (chunk_offset): ";
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

        const std::vector<uint32_t>& entries() const
        {
            return m_entries;
        }

        uint32_t entry_count() const
        {
            return m_entry_count;
        }


        uint32_t chunk_offset(uint32_t chunk_index) const
        {
            assert(chunk_index < m_entry_count);
            return m_entries[chunk_index];
        }


    private:

        /// an integer that gives the number of entries in the following table
        uint32_t m_entry_count;

        /// a vector containing each entry's chunk_offset.
        /// the chunk_offset is a 32 bit integer that gives the offset of the
        /// start of a chunk into its containing media file.
        std::vector<uint32_t> m_entries;

    };
}
}
