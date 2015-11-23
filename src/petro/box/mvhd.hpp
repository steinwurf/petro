// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <cassert>

#include "full_box.hpp"
#include "../byte_stream.hpp"
#include "../utils.hpp"

namespace petro
{
namespace box
{
    /// movie header, overall declarations
    class mvhd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        mvhd():
            full_box(mvhd::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(size, bs, parent);
            if (m_version == 1)
            {
                m_creation_time = bs.read_uint64_t();
                m_modification_time = bs.read_uint64_t();
                m_timescale = bs.read_uint32_t();
                m_duration = bs.read_uint64_t();
                m_remaining_bytes -= 28;
            }
            else
            {
                assert(m_version == 0);
                m_creation_time = bs.read_uint32_t();
                m_modification_time = bs.read_uint32_t();
                m_timescale = bs.read_uint32_t();
                m_duration = bs.read_uint32_t();
                m_remaining_bytes -= 16;
            }

            m_rate = bs.read_fixed_point(16, 16);
            m_remaining_bytes -= 4;
            m_volume = bs.read_fixed_point(8, 8);
            m_remaining_bytes -= 2;

            // reserved
            bs.skip(2 + 4 + 4);
            m_remaining_bytes -= 2 + 4 + 4;

            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_matrix.push_back(bs.read_int32_t());
            m_remaining_bytes -= 4 * 9;

            // pre_defined
            bs.skip(6 * 4);
            m_remaining_bytes -= 6 * 4;

            m_next_track_id = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            assert(m_remaining_bytes == 0);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  creation_time: " << parse_time(m_creation_time)
               << std::endl;
            ss << "  modification_time: " << parse_time(m_modification_time)
               << std::endl;
            ss << "  time_scale: " << m_timescale << std::endl;
            ss << "  duration: " << m_duration << std::endl;
            ss << "  rate: " << m_rate << std::endl;
            ss << "  volume: " << m_volume << std::endl;
            ss << "  next_track_id: " << m_next_track_id << std::endl;
            return ss.str();
        }

    private:

        /// an integer that declares the creation time of the presentation
        /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
        uint64_t m_creation_time;

        /// an integer that declares the most recent time the presentation was
        /// modified (in seconds since midnight, Jan. 1, 1904, in UTC time)
        uint64_t m_modification_time;

        /// an integer that specifies the time-scale for the entire
        /// presentation; this is the number of time units that pass in one
        /// second. For example, a time coordinate system that measures time in
        /// sixtieths of a second has a time scale of 60.
        uint32_t m_timescale;

        /// an integer that declares length of the presentation
        /// (in the indicated timescale). This property is derived from the
        /// presentation's tracks: the value of this field corresponds to the
        /// duration of the longest track in the presentation.
        uint64_t m_duration;

        /// a fixed point 16.16 number that indicates the preferred rate to play
        /// the presentation; 1.0 (0x00010000) is normal forward playback
        int32_t m_rate;

        /// is a fixed point 8.8 number that indicates the preferred playback
        /// volume. 1.0 (0x0100) is full volume.
        int16_t m_volume;

        /// provides a transformation matrix for the video; (u,v,w) are
        /// restricted here to (0,0,1), hex values (0,0,0x40000000).
        std::vector<int32_t> m_matrix;

        /// a non-zero integer that indicates a value to use for the track ID of
        /// the next track to be added to this presentation. Zero is not a valid
        /// track ID value. The value of next_track_ID shall be larger than the
        /// largest track-ID in use. If this value is equal to or larger than
        /// all 1s (32-bit maxint), and a new media track is to be added, then a
        /// search must be made in the file for a unused track identifier.
        uint32_t m_next_track_id;

    };

    const std::string mvhd::TYPE = "mvhd";
}
}
