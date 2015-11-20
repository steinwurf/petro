// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"
#include "../utils.hpp"

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
        mdhd(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(mdhd::TYPE, size, bs, parent)
        {
            if (m_version == 1)
            {
                m_creation_time = bs.read_uint64_t();
                m_modification_time = bs.read_uint64_t();
                m_timescale = bs.read_uint32_t();
                m_duration = bs.read_uint64_t();
                m_remaining_bytes -= 28;
            }
            else // m_version == 0
            {
                m_creation_time = bs.read_uint32_t();
                m_modification_time = bs.read_uint32_t();
                m_timescale = bs.read_uint32_t();
                m_duration = bs.read_uint32_t();
                m_remaining_bytes -= 16;
            }

            // ISO-639-2/T language code
            m_language = bs.read_iso639_code();
            m_remaining_bytes -= 2;

            // pre_defined
            bs.skip(2);
            m_remaining_bytes -= 2;

            bs.skip(m_remaining_bytes);
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
            ss << "  language: " << m_language << std::endl;
            return ss.str();
        }

    private:

        /// an integer that declares the creation time of the media in this
        /// track (in seconds since midnight, Jan. 1, 1904, in UTC time)
        uint64_t m_creation_time;

        /// an integer that declares the most recent time the media in this
        /// track was modified
        /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
        uint64_t m_modification_time;

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

    const std::string mdhd::TYPE = "mdhd";
}
}
