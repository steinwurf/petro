// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"
#include "../matrix.hpp"

namespace petro
{
namespace box
{
    /// track header, overall information about the track
    class tkhd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        tkhd(std::weak_ptr<box> parent):
            full_box(tkhd::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            full_box::read(size, bs);

            m_track_enabled = (m_flags.m_data[2] & 0x01) != 0;
            m_track_in_movie = (m_flags.m_data[2] & 0x02) != 0;
            m_track_in_preview = (m_flags.m_data[2] & 0x04) != 0;

            if (m_version == 1)
            {
                m_creation_time = bs.read_time64();
                m_modification_time = bs.read_time64();
                m_track_id = bs.read_uint32_t();

                // reserved
                bs.skip(4);

                m_duration = bs.read_uint64_t();
                m_remaining_bytes -= 32;
            }
            else  // m_version == 0
            {
                m_creation_time = bs.read_time32();
                m_modification_time = bs.read_time32();
                m_track_id = bs.read_uint32_t();

                // reserved
                bs.skip(4);

                m_duration = bs.read_uint32_t();
                m_remaining_bytes -= 20;
            }
            // reserved
            bs.skip(8);
            m_remaining_bytes -= 8;

            m_layer = bs.read_int16_t();
            m_remaining_bytes -= 2;

            m_alternate_group = bs.read_int16_t();
            m_remaining_bytes -= 2;

            m_volume = bs.read_fixed_point_88();
            m_remaining_bytes -= 2;

            // reserved
            bs.skip(2);
            m_remaining_bytes -= 2;

            m_matrix.read(bs);
            m_remaining_bytes -= 4 * 9;

            m_width = bs.read_fixed_point_1616();
            m_remaining_bytes -= 4;

            m_height = bs.read_fixed_point_1616();
            m_remaining_bytes -= 4;
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  track_enabled: " << m_track_enabled << std::endl;
            ss << "  track_in_movie: " << m_track_in_movie << std::endl;
            ss << "  track_in_preview: " << m_track_in_preview << std::endl;
            ss << "  creation_time: " << m_creation_time << std::endl;
            ss << "  modification_time: " << m_modification_time << std::endl;
            ss << "  track_id: " << m_track_id << std::endl;
            ss << "  duration: " << m_duration << std::endl;
            ss << "  layer: " << m_layer << std::endl;
            ss << "  alternate_group: " << m_alternate_group << std::endl;
            ss << "  volume: " << m_volume << std::endl;
            ss << "  matrix: " << std::endl << m_matrix.describe();
            ss << "  width: " << m_width << std::endl;
            ss << "  height: " << m_height << std::endl;
            return ss.str();
        }

        bool track_enabled() const
        {
            return m_track_enabled;
        }

        bool track_in_movie() const
        {
            return m_track_in_movie;
        }

        bool track_in_preview() const
        {
            return m_track_in_preview;
        }

        std::string creation_time() const
        {
            return m_creation_time;
        }

        std::string modification_time() const
        {
            return m_modification_time;
        }

        uint32_t track_id() const
        {
            return m_track_id;
        }

        uint64_t duration() const
        {
            return m_duration;
        }

        int16_t layer() const
        {
            return m_layer;
        }

        int16_t alternate_group() const
        {
            return m_alternate_group;
        }

        float volume() const
        {
            return m_volume;
        }

        petro::matrix matrix() const
        {
            return m_matrix;
        }

        double width() const
        {
            return m_width;
        }

        double height() const
        {
            return m_height;
        }

    private:

        /// indicates that the track is enabled.
        bool m_track_enabled;

        /// indicates that the track is used in the presentation.
        bool m_track_in_movie;

        /// indicates that the track is used when previewing the presentation.
        bool m_track_in_preview;

        /// an integer that declares the creation time of this track
        /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
        std::string m_creation_time;

        /// an integer that declares the most recent time the track was modified
        /// (in seconds since midnight, Jan. 1, 1904, in UTC time)
        std::string m_modification_time;

        /// an integer that uniquely identifies this track over the entire
        /// life-time of this presentation.
        /// Track IDs are never re-used and cannot be zero.
        uint32_t m_track_id;

        /// an integer that indicates the duration of this track
        /// (in the timescale indicated in the Movie Header Box).
        /// The value of this field is equal to the sum of the durations of all
        /// of the track’s edits. If there is no edit list, then the duration
        /// is the sum of the sample durations, converted into the timescale
        /// in the Movie Header Box. If the duration of this track cannot be
        /// determined then duration is set to all 1s (32-bit maxint).
        uint64_t m_duration;

        /// specifies the front-to-back ordering of video tracks; tracks with
        /// lower numbers are closer to the viewer. 0 is the normal value,
        /// and -1 would be in front of track 0, and so on.
        int16_t m_layer;

        /// is an integer that specifies a group or collection of tracks.
        /// If this field is 0 there is no information on possible relations to
        /// other tracks. If this field is not 0, it should be the same for
        /// tracks that contain alternate data for one another and different for
        /// tracks belonging to different such groups. Only one track within an
        /// alternate group should be played or streamed at any one time, and
        /// must be distinguishable from other tracks in the group via
        /// attributes such as bitrate, codec, language, packet size etc.
        /// A group may have only one member.
        int16_t m_alternate_group;

        /// a fixed 8.8 value specifying the track's relative audio volume.
        /// Full volume is 1.0 (0x0100) and is the normal value. Its value is
        /// irrelevant for a purely visual track. Tracks may be composed by
        /// combining them according to their volume, and then using the overall
        /// Movie Header Box volume setting; or more complex audio composition
        /// (e.g. MPEG-4 BIFS) may be used.
        float m_volume;

        /// provides a transformation matrix for the video; (u,v,w) are
        /// restricted here to (0,0,1), hex (0,0,0x40000000).
        petro::matrix m_matrix;

        /// specifies the track's visual presentation size as fixed-point 16.16
        /// values. These need not be the same as the pixel dimensions of the
        /// images, which is documented in the sample description(s);
        /// all images in the sequence are scaled to this size, before any
        /// overall transformation of the track represented by the matrix.
        /// The pixel dimensions of the images are the default values.
        double m_width;
        double m_height;
    };
}
}
