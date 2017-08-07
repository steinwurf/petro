// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "avcc.hpp"
#include "esds.hpp"
#include "full_box.hpp"
#include "hdlr.hpp"
#include "../byte_stream.hpp"
#include "../parser.hpp"

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

        sample_entry(const uint8_t* data, uint64_t size) :
            box(data, size)
        { }

        void parse_box_content(std::error_code& error) override final
        {
            assert(!error);

            // reserved
            m_bs.skip(6, error);
            if (error)
                return;

            m_bs.read(m_data_reference_index, error);
            if (error)
                return;

            parse_sample_entry(error);
            if (error)
                return;
        }

        virtual void parse_sample_entry(std::error_code& error)
        {
            assert(!error);
            m_bs.skip(m_bs.remaining_size(), error);
            if (error)
                return;
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

        visual_sample_entry(const uint8_t* data, uint64_t size) :
            sample_entry(data, size)
        { }

        void parse_sample_entry(std::error_code& error) override
        {
            // pre_defined
            m_bs.skip(2, error);
            if (error)
                return;

            // reserved
            m_bs.skip(2, error);
            if (error)
                return;

            // pre_defined
            m_bs.skip(4 * 3, error);
            if (error)
                return;

            m_bs.read(m_width, error);
            if (error)
                return;

            m_bs.read(m_height, error);
            if (error)
                return;

            m_bs.read_fixed_point_1616(m_horizontal_resolution, error);
            if (error)
                return;

            m_bs.read_fixed_point_1616(m_vertical_resolution, error);
            if (error)
                return;

            // reserved
            m_bs.skip(4, error);
            if (error)
                return;

            m_bs.read(m_frame_count, error);
            if (error)
                return;

            for (uint8_t i = 0; i < 32; ++i)
            {
                uint8_t c;
                m_bs.read(c, error);
                if (error)
                    return;
                m_compressor_name += c;
            }

            m_bs.read(m_depth, error);
            if (error)
                return;

            // pre_defined
            m_bs.skip(2, error);
            if (error)
                return;

            parser<avcc> p;
            p.parse(
                m_bs.remaining_data(),
                m_bs.remaining_size(),
                shared_from_this(), error);
            if (error)
                return;

            m_bs.skip(m_bs.remaining_size(), error);
            if (error)
                return;
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
        double m_horizontal_resolution;
        double m_vertical_resolution;

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

        audio_sample_entry(const uint8_t* data, uint64_t size) :
            sample_entry(data, size)
        { }

        void parse_sample_entry(std::error_code& error) override
        {
            // reserved
            m_bs.skip(4 * 2, error);
            if (error)
                return;

            m_bs.read(m_channel_count, error);
            if (error)
                return;

            m_bs.read(m_sample_size, error);
            if (error)
                return;

            // pre_defined
            m_bs.skip(2, error);
            if (error)
                return;

            // reserved
            m_bs.skip(2, error);
            if (error)
                return;

            // {timescale of media}<<16;
            m_bs.read_fixed_point_1616(m_sample_rate, error);
            if (error)
                return;

            parser<esds> p;
            p.parse(
                m_bs.remaining_data(),
                m_bs.remaining_size(),
                shared_from_this(), error);
            if (error)
                return;

            m_bs.skip(m_bs.remaining_size(), error);
            if (error)
                return;
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

        double sample_rate() const
        {
            return m_sample_rate;
        }

    private:

        /// either 1 (mono) or 2 (stereo)
        uint16_t m_channel_count;

        /// sample size in bits
        uint16_t m_sample_size;

        /// the sampling rate expressed as a 16.16 fixed-point number
        /// (hi.lo)
        double m_sample_rate;
    };

    class hint_sample_entry : public sample_entry
    {

    public:

        hint_sample_entry(const uint8_t* data, uint64_t size) :
            sample_entry(data, size)
        { }

        void parse_sample_entry(std::error_code& error) override
        {
            m_data.resize(m_bs.remaining_size());
            m_bs.read(m_data.data(), m_data.size(), error);
            if (error)
                return;
        }

        std::string describe() const
        {
            std::stringstream ss;
            ss << sample_entry::describe() << std::endl;
            ss << "    data: " << (uint64_t)m_data.data() << std::endl;
            ss << "    data.size: " << m_data.size() << std::endl;
            return ss.str();
        }

    private:

        std::vector<uint8_t> m_data;
    };

public:

    static const std::string TYPE;

public:

    stsd(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read(m_entry_count, error);
        if (error)
            return;

        // get handler to know which kind of sample we are reading.
        auto mdia = get_parent("mdia");
        if (mdia == nullptr)
        {
            error = std::make_error_code(std::errc::not_supported);
            return;
        }
        // auto hdlr = mdia->get_child<petro::box::hdlr>();
        // if (hdlr == nullptr)
        // {
        //     error = std::make_error_code(std::errc::not_supported);
        //     return;
        // }


        // for (uint32_t i = 0; i < m_entry_count; ++i)
        // {
        //     // Skip the size, we are only interested in the type
        //     m_bs.skip(4);
        //     if (error)
        //         return;

        //     uint32_t entry_type_value;
        //     m_bs.read(entry_type_value, error);
        //     if (error)
        //         return;

        //     auto entry_type = helper::type(entry_type_value);

        //     std::shared_ptr<sample_entry> entry = nullptr;
        //     if (hdlr->handler_type() == "vide") // for video tracks
        //     {
        //         entry = std::make_shared<visual_sample_entry>(
        //                 m_bs.remaining_data(),
        //                 m_bs.remaining_size());
        //         entry->parse(error);
        //     }
        //     else if (hdlr->handler_type() == "soun") // for audio tracks
        //     {
        //         entry = std::make_shared<audio_sample_entry>(
        //                 m_bs.remaining_data(),
        //                 m_bs.remaining_size());
        //         entry->parse(error);
        //     }
        //     else if (hdlr->handler_type() == "hint")
        //     {
        //         entry = std::make_shared<hint_sample_entry>(
        //                 m_bs.remaining_data(),
        //                 m_bs.remaining_size());
        //         entry->parse(error);
        //     }
        //     else
        //     {
        //         error = std::make_error_code(std::errc::not_supported);
        //     }

        //     if (error)
        //         return;

        //     entry->set_parent(shared_from_this());

        //     m_bs.skip(entry->size());
        //     if (error)
        //         return;
        //     m_children.push_back(entry);
        // }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    // void read(uint32_t size, byte_stream& bs)
    // {
        // full_box::read(size, bs);
        // m_entry_count = bs.read_uint32_t();
        // m_remaining_bytes -= 4;

        // // get handler to know which kind of sample we are reading.
        // auto mdia = get_parent("mdia");
        // assert(mdia != nullptr);
        // auto hdlr = std::dynamic_pointer_cast<const petro::box::hdlr>(
        //     mdia->get_child("hdlr"));

        // for (uint32_t i = 0; i < m_entry_count; ++i)
        // {
        //     uint32_t entry_size = bs.read_uint32_t();
        //     std::string entry_type = helper::type(bs.read_uint32_t());

        //     std::shared_ptr<sample_entry> entry = nullptr;
        //     if (hdlr != nullptr)
        //     {
        //         std::string handler_type = hdlr->handler_type();
        //         if (handler_type == "vide") // for video tracks
        //         {
        //             entry = std::make_shared<visual_sample_entry>(entry_type, shared_from_this());
        //             entry->read(entry_size, bs);
        //         }
        //         else if (handler_type == "soun") // for audio tracks
        //         {
        //             entry = std::make_shared<audio_sample_entry>(entry_type, shared_from_this());
        //             entry->read(entry_size, bs);
        //         }
        //         else if (handler_type == "hint")
        //         {
        //             entry = std::make_shared<hint_sample_entry>(entry_type, shared_from_this());
        //             entry->read(entry_size, bs);
        //         }
        //     }
        //     if (!entry)
        //     {
        //         entry = std::make_shared<unknown_sample_entry>(entry_type, shared_from_this());
        //         entry->read(entry_size, bs);
        //     }

        //     m_remaining_bytes -= entry->size();
        //     m_children.push_back(entry);
        // }
        // bs.skip(m_remaining_bytes);
    // }

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
}
}
