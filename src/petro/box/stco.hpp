// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

    stco(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_entry_count, error);
        if (error)
            return;
        for (uint32_t i = 0; i < m_entry_count; ++i)
        {
            uint32_t chunk_offset = 0;
            m_bs.read(chunk_offset, error);
            if (error)
                return;
            m_entries.push_back(chunk_offset);
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        ss << "  entry_count: " << m_entry_count << std::endl;
        ss << "  entries (chunk_offset): ";
        auto seperator = "";
        uint32_t max_print = 5;
        for (uint32_t i = 0;
             i < std::min((uint32_t)m_entries.size(), max_print); ++i)
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

    const std::vector<uint64_t>& entries() const
    {
        return m_entries;
    }

    uint32_t entry_count() const
    {
        return m_entry_count;
    }


    uint64_t chunk_offset(uint32_t chunk_index) const
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
    std::vector<uint64_t> m_entries;

};
}
}
