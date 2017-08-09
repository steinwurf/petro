// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include "full_box.hpp"

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
    stss(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_entry_count, error);
        if (error)
            return;
        for (uint32_t i = 0; i < m_entry_count; ++i)
        {
            uint32_t entry_value;
            m_bs.read(entry_value, error);
            if (error)
                return;
            m_entries.push_back(entry_value);
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        ss << "  entry_count: " << m_entry_count << std::endl;
        ss << "  entries (sample_number): ";
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

    const std::vector<uint32_t>& entries() const
    {
        return m_entries;
    }

    bool is_sample_random_access_point(uint32_t sample_index) const
    {
        auto sample_number = sample_index + 1;
        auto result = std::find(
            m_entries.begin(), m_entries.end(), sample_number);
        return result != m_entries.end();
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
}
}
