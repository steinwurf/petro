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
    /// an edit list
    class elst : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(elst::TYPE, size, bs, parent);
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                uint64_t segment_duration;
                int64_t media_time;
                if (m_version == 1)
                {
                    segment_duration = bs.read_uint64_t();
                    media_time = bs.read_int64_t();
                    m_remaining_bytes -= 16;
                }
                else // m_version == 0
                {
                    segment_duration = bs.read_uint32_t();
                    media_time = bs.read_int32_t();
                    m_remaining_bytes -= 8;
                }
                m_entries.push_back(
                    std::tuple<uint64_t,int64_t,uint16_t,uint16_t>(
                        segment_duration,
                        media_time,
                        bs.read_uint16_t(),  // media_rate_integer
                        bs.read_uint16_t())); // media_rate_fraction
                m_remaining_bytes -= 4;
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (";
            ss << "segment_duration, ";
            ss << "media_time, ";
            ss << "media_rate_integer, ";
            ss << "media_rate_fraction): ";
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
                    << std::get<2>(entry) << ","
                    << std::get<3>(entry) << ")";
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

        /// a vector containg tuples with the following attibutes:
        /// segment_duration, an integer that specifies the duration of this
        /// edit segment in units of the timescale in the mvhd.
        /// media_time, an integer containing the starting time within the
        /// media of this edit segment (in media time scale units, in
        /// composition time). If this field is set to –1, it is an empty edit.
        /// The last edit in a track shall never be an empty edit. Any
        /// difference between the duration in the mvhd, and
        /// the track's duration is expressed as an implicit empty edit at the
        /// end.
        /// media_rate specifies the relative rate at which to play the media
        /// corresponding to this edit segment. If this value is 0, then the
        /// edit is specifying a ‘dwell’: the media at media-time is presented
        /// for the segment-duration.
        /// Otherwise this field shall contain the value 1.
        std::vector<std::tuple<uint64_t,int64_t,uint16_t,uint16_t>> m_entries;
    };

    const std::string elst::TYPE = "elst";
}
}
