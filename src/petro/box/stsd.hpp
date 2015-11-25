// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "hdlr.hpp"
#include "../byte_stream.hpp"
#include "../parser.hpp"
#include "../utils.hpp"

namespace petro
{
namespace box
{
    /// sample descriptions (codec types, initialization etc.)
    class stsd : public full_box
    {

    public:

        class sample_entry : public box
        {

        public:

            sample_entry(const std::string& format, std::weak_ptr<box> parent):
                box(format, parent)
            { }

            void read(uint32_t size, byte_stream& bs)
            {
                box::read(size, bs);
                // reserved
                bs.skip(6);
                m_remaining_bytes -= 6;

                m_data_reference_index = bs.read_uint16_t();
                m_remaining_bytes -= 2;
            }

            virtual std::string describe() const
            {
                std::stringstream ss;
                ss << box::describe() << std::endl;
                ss << "  data_reference_index: " << m_data_reference_index;
                return ss.str();
            }

        private:

            /// an integer that contains the index of the data reference to use
            /// to retrieve data associated with samples that use this sample
            /// description. Data references are stored in dref boxes. The index
            /// ranges from 1 to the number of data references.
            uint32_t m_data_reference_index;
        };

        class visual_sample_entry : public sample_entry
        {

        public:

            visual_sample_entry(const std::string& coding_name,
                std::weak_ptr<box> parent):
                sample_entry(coding_name, parent)
            { }

            void read(uint32_t size, byte_stream& bs)
            {
                sample_entry::read(size, bs);
                // pre_defined
                bs.skip(2);
                m_remaining_bytes -= 2;

                // reserved
                bs.skip(2);
                m_remaining_bytes -= 2;

                // pre_defined
                bs.skip(4 * 3);
                m_remaining_bytes -= 4 * 3;

                m_width = bs.read_uint16_t();
                m_height = bs.read_uint16_t();
                m_horizontal_resolution = bs.read_fixed_point(16, 16);
                m_vertical_resolution = bs.read_fixed_point(16, 16);
                m_remaining_bytes -= 2 + 2 + 4 + 4;

                // reserved
                bs.skip(4);
                m_remaining_bytes -= 4;

                m_frame_count = bs.read_uint16_t();
                m_remaining_bytes -= 2;
                for (uint8_t i = 0; i < 32; ++i)
                {
                    m_compressor_name += bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }

                m_depth = bs.read_uint16_t();
                m_remaining_bytes -= 2;

                // pre_defined
                bs.skip(2);
                m_remaining_bytes -= 2;

                parser<avcc> p;
                p.read(shared_from_this(), bs.data(), m_remaining_bytes);
                bs.skip(m_remaining_bytes);
            }

            std::string describe() const
            {
                std::stringstream ss;
                ss << sample_entry::describe() << std::endl;
                ss << "  width: " << m_width << std::endl;
                ss << "  height: " << m_height << std::endl;
                ss << "  horizontal_resolution: " << m_horizontal_resolution
                   << std::endl;
                ss << "  vertical_resolution: " << m_vertical_resolution
                   << std::endl;
                ss << "  frame_count: " << m_frame_count << std::endl;
                ss << "  compressor_name: " << m_compressor_name << std::endl;
                ss << "  depth: " << m_depth;
                return ss.str();
            }

        private:

            /// are the maximum visual width and height of the stream described
            /// by this sample description, in pixels
            uint16_t m_width;
            uint16_t m_height;

            /// fields give the resolution of the image in pixels-per-inch, as a
            /// fixed 16.16 number
            uint32_t m_horizontal_resolution;
            uint32_t m_vertical_resolution;

            /// how many frames of compressed video are stored in each sample.
            /// The default is 1, for one frame per sample; it may be more than
            /// 1 for multiple frames per sample
            uint16_t m_frame_count;

            /// a name, for informative purposes. It is formatted in a fixed
            /// 32-byte field, with the first byte set to the number of bytes
            /// to be displayed, followed by that number of bytes of displayable
            /// data, and then padding to complete 32 bytes total (including
            /// the size byte). The field may be set to 0.
            std::string m_compressor_name;

            /// takes one of the following values
            ///     0x0018 – images are in colour with no alpha
            uint16_t m_depth;
        };

        class audio_sample_entry : public sample_entry
        {

        public:

            audio_sample_entry(const std::string& coding_name, std::weak_ptr<box> parent):
                sample_entry(coding_name, parent)
            { }

            void read(uint32_t size, byte_stream& bs)
            {
                sample_entry::read(size, bs);
                // reserved
                bs.skip(4 * 2);
                m_remaining_bytes -= 4 * 2;

                m_channel_count = bs.read_uint16_t();
                m_sample_size = bs.read_uint16_t();
                m_remaining_bytes -= 4;

                // pre_defined
                bs.skip(2);
                m_remaining_bytes -= 2;

                // reserved
                bs.skip(2);
                m_remaining_bytes -= 2;

                // {timescale of media}<<16;
                m_sample_rate = bs.read_fixed_point(16, 16);
                m_remaining_bytes -= 4;
                bs.skip(m_remaining_bytes);
            }

            std::string describe() const
            {
                std::stringstream ss;
                ss << sample_entry::describe() << std::endl;
                ss << "  channel_count: " << m_channel_count << std::endl;
                ss << "  sample_size: " << m_sample_size << std::endl;
                ss << "  sample_rate: " << m_sample_rate;
                return ss.str();
            }

        private:

            /// either 1 (mono) or 2 (stereo)
            uint16_t m_channel_count;

            /// sample size in bits
            uint16_t m_sample_size;

            /// the sampling rate expressed as a 16.16 fixed-point number
            /// (hi.lo)
            uint32_t m_sample_rate;
        };

        class hint_sample_entry : public sample_entry
        {

        public:

            hint_sample_entry(const std::string& protocol, std::weak_ptr<box> parent):
                sample_entry(protocol, parent)
            { }

            void read(uint32_t size, byte_stream& bs)
            {
                sample_entry::read(size, bs);
                bs.skip(m_remaining_bytes);
            }

            std::string describe() const
            {
                std::stringstream ss;
                ss << sample_entry::describe() << std::endl;
                ss << "    data: " << (uint64_t)m_data << std::endl;
                return ss.str();
            }

        private:

            uint8_t* m_data = nullptr;
        };

        class unknown_sample_entry : public sample_entry
        {

        public:

            unknown_sample_entry(const std::string& format, std::weak_ptr<box> parent):
                sample_entry(format, parent)
            { }

            void read(uint32_t size, byte_stream& bs)
            {
                sample_entry::read(size, bs);
                bs.skip(m_remaining_bytes);
            }
        };

    public:

        static const std::string TYPE;

    public:
        stsd(std::weak_ptr<box> parent):
            full_box(stsd::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            full_box::read(size, bs);
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            auto mdia = get_parent("mdia");
            assert(mdia != nullptr);
            auto hdlr =
                std::dynamic_pointer_cast<const petro::box::hdlr>(mdia->get_child("hdlr"));
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                uint32_t entry_size = bs.read_uint32_t();
                std::string entry_type = bs.read_type();

                std::shared_ptr<sample_entry> entry = nullptr;
                if (hdlr != nullptr)
                {
                    std::string handler_type = hdlr->handler_type();
                    if (handler_type == "vide") // for video tracks
                    {
                        entry = std::make_shared<visual_sample_entry>(entry_type, shared_from_this());
                        entry->read(entry_size, bs);
                    }
                    else if (handler_type == "soun") // for audio tracks
                    {
                        entry = std::make_shared<audio_sample_entry>(entry_type, shared_from_this());
                        entry->read(entry_size, bs);
                    }
                    else if (handler_type == "hint")
                    {
                        entry = std::make_shared<hint_sample_entry>(entry_type, shared_from_this());
                        entry->read(entry_size, bs);
                    }
                }
                if (!entry)
                {
                    entry = std::make_shared<unknown_sample_entry>(entry_type, shared_from_this());
                    entry->read(entry_size, bs);
                }

                m_remaining_bytes -= entry->size();
                m_children.push_back(entry);
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            return ss.str();
        }

    private:

        /// an integer that gives the number of entries
        uint32_t m_entry_count;
    };

    const std::string stsd::TYPE = "stsd";
}
}
