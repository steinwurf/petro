// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

namespace petro
{
namespace box
{
/// media header, overall information about the media
class mdhd : public full_box
{

public:

    static const std::string TYPE;

public:

    mdhd(const uint8_t* data, uint64_t size) :
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

            m_bs.read<uint32_t>(m_timescale, error);
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

            m_bs.read<uint32_t>(m_timescale, error);
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
            error = box_error_code();
            return;
        }

        // ISO-639-2/T language code
        m_bs.read_iso639(m_language, error);
        if (error)
            return;

        // pre_defined
        m_bs.skip(2, error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_mdhd_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        ss << "  creation_time: " << m_creation_time << std::endl;
        ss << "  modification_time: " << m_modification_time << std::endl;
        ss << "  time_scale: " << m_timescale << std::endl;
        ss << "  duration: " << m_duration << std::endl;
        ss << "  language: " << m_language << std::endl;
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

    std::string language() const
    {
        return m_language;
    }

private:

    /// an integer that declares the creation time of the media in this
    /// track (in seconds since midnight, Jan. 1, 1904, in UTC time)
    std::string m_creation_time;

    /// an integer that declares the most recent time the media in this
    /// track was modified
    /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
    std::string m_modification_time;

    /// an integer that specifies the time-scale for this media; this is
    /// the number of time units that pass in one second.
    /// For example, a time coordinate system that measures time in
    /// sixtieths of a second has a time scale of 60.
    uint32_t m_timescale;

    /// an integer that declares the duration of this media
    /// (in the scale of the timescale).
    uint64_t m_duration;

    /// declares the language code for this media. See ISO 639-2/T for the
    /// set of three character codes. Each character is packed as the
    /// difference between its ASCII value and 0x60. Since the code is
    /// confined to being three lower-case letters, these values are
    /// strictly positive.
    std::string m_language;
};
}
}
