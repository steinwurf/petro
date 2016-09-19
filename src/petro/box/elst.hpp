// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

            struct entry_type
            {

                /// specifies the duration of this edit segment in units of the
                /// timescale in the mvhd.
                uint64_t segment_duration;

                /// The starting time within the media of this edit segment
                /// (in media time scale units, in composition time).
                /// If this field is set to -1, it is an empty edit.
                /// The last edit in a track shall never be an empty edit. Any
                /// difference between the duration in the mvhd, and the track's
                /// duration is expressed as an implicit empty edit at the end.
                int64_t media_time;

                /// the relative rate at which to play the media corresponding to
                /// this edit segment.
                /// If this value is 0, then the edit is specifying a "dwell":
                /// the media at media-time is presented for the segment-duration.
                /// Otherwise this field shall contain the value 1.
                uint16_t media_rate_integer;
                uint16_t media_rate_fraction;
            };

        public:

            static const std::string TYPE;

        public:
            elst(std::weak_ptr<box> parent) :
                full_box(elst::TYPE, parent)
            { }

            void read(uint64_t size, byte_stream& bs)
            {
                full_box::read(size, bs);
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
                    {
                        segment_duration,
                        media_time,
                        bs.read_uint16_t(),  // media_rate_integer
                        bs.read_uint16_t()
                    }); // media_rate_fraction
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
                       << entry.segment_duration << ","
                       << entry.media_time << ","
                       << entry.media_rate_integer << ","
                       << entry.media_rate_fraction << ")";
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

            /// a vector containing each entry's segment duration, media time,
            /// media rate integer, and media rate fraction.
            std::vector<entry_type> m_entries;
        };
    }
}
