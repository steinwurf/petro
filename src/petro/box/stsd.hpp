// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "hdlr.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{


    const box* get_grand_parent(const box* b, const std::string& grand_parent)
    {
        const box* parent = b->parent();
        if (parent != nullptr)
        {
            for (auto child : parent->children())
            {
                if (child->type() == grand_parent)
                {
                    return child;
                }
            }
            if (parent->parent() != nullptr)
            {
                return get_grand_parent(parent, grand_parent);
            }
        }
        return nullptr;
    }

    /// sample descriptions (codec types, initialization etc.)
    class stsd : public full_box
    {

    public:

        class sample_entry : public box
        {

        public:

            sample_entry(const std::string& format, uint32_t size,
                byte_stream& bs, box* parent):
                box(format, size, bs, parent)
            {
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

            uint32_t m_data_reference_index;
        };

        class visual_sample_entry : public sample_entry
        {

        public:

            visual_sample_entry(const std::string& coding_name, uint32_t size,
                byte_stream& bs, box* parent):
                sample_entry(coding_name, size, bs, parent)
            {
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
                m_horizontal_resolution = bs.read_uint32_t();
                m_vertical_resolution = bs.read_uint32_t();
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

            uint16_t m_width;
            uint16_t m_height;
            uint32_t m_horizontal_resolution;
            uint32_t m_vertical_resolution;
            uint16_t m_frame_count;
            std::string m_compressor_name;
            uint16_t m_depth;
        };

        class audio_sample_entry : public sample_entry
        {

        public:

            audio_sample_entry(const std::string& coding_name, uint32_t size,
                byte_stream& bs, box* parent):
                sample_entry(coding_name, size, bs, parent)
            {
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

            uint16_t m_channel_count;
            uint16_t m_sample_size;
            uint32_t m_sample_rate;
        };

        class hint_sample_entry : public sample_entry
        {

        public:

            hint_sample_entry(const std::string& protocol, uint32_t size,
                byte_stream& bs, box* parent):
                sample_entry(protocol, size, bs, parent)
            {
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

    public:

        static const std::string TYPE;

    public:
        stsd(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(stsd::TYPE, size, bs, parent)
        {
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;

            const box* handler = get_grand_parent(this, "hdlr");
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                uint32_t entry_size = bs.read_uint32_t();
                std::string entry_type = bs.read_type();

                sample_entry* entry;
                if (handler == nullptr)
                {
                    entry = new sample_entry(entry_type, entry_size, bs, this);
                }
                else
                {
                    std::string handler_type = ((hdlr*)handler)->handler_type();
                    if (handler_type == "vide") // for video tracks
                    {
                        entry = new visual_sample_entry(
                            entry_type, entry_size, bs, this);
                    }
                    else if (handler_type == "soun") // for audio tracks
                    {
                        entry = new audio_sample_entry(
                            entry_type, entry_size, bs, this);
                    }
                    else if (handler_type == "hint")
                    {
                        entry = new hint_sample_entry(
                            entry_type, entry_size, bs, this);
                    }
                    else
                    {
                        // unkown handler_type.
                        assert(0);
                    }
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
