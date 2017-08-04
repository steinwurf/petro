// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <cassert>

#include "full_box.hpp"
#include "../matrix.hpp"
#include "../byte_stream.hpp"

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

    mvhd(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
        if (m_version == 1)
        {
            m_creation_time = helper::time64(bs.read_uint64_t());
            m_modification_time = helper::time64(bs.read_uint64_t());
            m_timescale = bs.read_uint32_t();
            m_duration = bs.read_uint64_t();
            m_remaining_bytes -= 28;
        }
        else
        {
            assert(m_version == 0);
            m_creation_time = helper::time32(bs.read_uint32_t());
            m_modification_time = helper::time32(bs.read_uint32_t());
            m_timescale = bs.read_uint32_t();
            m_duration = bs.read_uint32_t();
            m_remaining_bytes -= 16;
        }

        m_rate = helper::fixed_point_1616(bs.read_uint32_t());
        m_remaining_bytes -= 4;
        m_volume = helper::fixed_point_88(bs.read_uint16_t());
        m_remaining_bytes -= 2;

        // reserved
        bs.skip(2 + 4 + 4);
        m_remaining_bytes -= 2 + 4 + 4;

        m_matrix.read(bs);
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
        ss << "  creation_time: " << m_creation_time << std::endl;
        ss << "  modification_time: " << m_modification_time << std::endl;
        ss << "  time_scale: " << m_timescale << std::endl;
        ss << "  duration: " << m_duration << std::endl;
        ss << "  rate: " << m_rate << std::endl;
        ss << "  volume: " << m_volume << std::endl;
        ss << "  matrix: " << std::endl << m_matrix.describe();
        ss << "  next_track_id: " << m_next_track_id << std::endl;
        return ss.str();
    }

    std::string creation_time() const
    {
        return m_creation_time;
    }

    std::string modification_time() const
    {
        return m_modification_time;
    }

    uint32_t timescale() const
    {
        return m_timescale;
    }

    uint64_t duration() const
    {
        return m_duration;
    }

    double rate() const
    {
        return m_rate;
    }

    float volume() const
    {
        return m_volume;
    }

    petro::matrix matrix() const
    {
        return m_matrix;
    }

    uint32_t next_track_id() const
    {
        return m_next_track_id;
    }


private:

    /// an integer that declares the creation time of the presentation
    /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
    std::string m_creation_time;

    /// an integer that declares the most recent time the presentation was
    /// modified (in seconds since midnight, Jan. 1, 1904, in UTC time)
    std::string m_modification_time;

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
    double m_rate;

    /// is a fixed point 8.8 number that indicates the preferred playback
    /// volume. 1.0 (0x0100) is full volume.
    float m_volume;

    /// provides a transformation matrix for the video; (u,v,w) are
    /// restricted here to (0,0,1), hex values (0,0,0x40000000).
    petro::matrix m_matrix;

    /// a non-zero integer that indicates a value to use for the track ID of
    /// the next track to be added to this presentation. Zero is not a valid
    /// track ID value. The value of next_track_ID shall be larger than the
    /// largest track-ID in use. If this value is equal to or larger than
    /// all 1s (32-bit maxint), and a new media track is to be added, then a
    /// search must be made in the file for a unused track identifier.
    uint32_t m_next_track_id;

};
}
}
