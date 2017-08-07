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

    void parse_full_box_content(std::error_code& error) override
    {
        if (m_version == 1)
        {
            m_bs.read_time64(m_creation_time, error);
            if (error)
                return;
            m_bs.read_time64(m_modification_time, error);
            if (error)
                return;
            m_bs.read(m_timescale, error);
            if (error)
                return;
            m_bs.read<uint64_t>(m_duration, error);
            if (error)
                return;
        }
        else if (m_version == 0)
        {


            m_bs.read_time32(m_creation_time, error);
            if (error)
                return;
            m_bs.read_time32(m_modification_time, error);
            if (error)
                return;
            m_bs.read(m_timescale, error);
            if (error)
                return;
            uint32_t duration = 0;
            m_bs.read<uint32_t>(duration, error);
            if (error)
                return;
            m_duration = duration;
        }
        else
        {
            error = std::make_error_code(std::errc::value_too_large);
            return;
        }

        m_bs.read_fixed_point_1616(m_rate, error);
        if (error)
            return;

        m_bs.read_fixed_point_88(m_volume, error);
        if (error)
            return;

        // reserved
        m_bs.skip(2 + 4 + 4, error);
        if (error)
            return;

        m_matrix.read(m_bs, error);
        if (error)
            return;

        // pre_defined
        m_bs.skip(6 * 4, error);
        if (error)
            return;

        m_bs.read(m_next_track_id, error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
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
