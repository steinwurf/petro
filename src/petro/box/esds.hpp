// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "full_box.hpp"
#include "../descriptor/elemetary_stream_descriptor.hpp"
namespace petro
{
namespace box
{
/// MPEG-4 Elementary Stream Descriptor
class esds : public full_box
{
public:

    static const std::string TYPE;

public:

    using descriptor_type =
        std::shared_ptr<descriptor::elemetary_stream_descriptor>;

public:

    esds(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        assert(!error);
        m_descriptor = std::make_shared<descriptor_type::element_type>(
            m_bs.remaining_data(), m_bs.remaining_size());
        m_descriptor->parse(error);
        if (error)
            return;

        m_bs.skip(m_descriptor->size(), error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_esds_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        std::string indent = "  ";
        ss << indent << "descriptor:" << std::endl;

        indent = "    ";

        ss << indent << "es_id: " << (uint32_t)m_descriptor->es_id() << std::endl;

        ss << indent << "byte_stream_priority: "
           << (uint32_t)m_descriptor->byte_stream_priority() << std::endl;

        ss << indent << "depends_on_esid: "
           << (uint32_t)m_descriptor->depends_on_esid() << std::endl;
        if (!m_descriptor->url().empty())
            ss << indent << "url: " << m_descriptor->url() << std::endl;

        ss << indent << "ocr_es_id: "
           << (uint32_t)m_descriptor->ocr_es_id() << std::endl;

        auto decoder_config_descriptor =
            m_descriptor->decoder_config_descriptor();

        ss << indent << "decoder_config_descriptor:" << std::endl;

        indent = "      ";

        ss << indent << "object_type_id: "
           << (uint32_t)decoder_config_descriptor->object_type_id()
           << std::endl;

        ss << indent << "max_bitrate: "
           << decoder_config_descriptor->max_bitrate() << std::endl;

        ss << indent << "average_bitrate: "
           << decoder_config_descriptor->average_bitrate() << std::endl;

        auto decoder_specific_info_descriptor =
            decoder_config_descriptor->decoder_specific_info_descriptor();

        if (decoder_specific_info_descriptor != nullptr)
        {
            ss << indent << "decoder_specific_info_descriptor:" << std::endl;

            indent = "        ";
            ss << indent << "mpeg_audio_object_type: "
               << (uint32_t)decoder_specific_info_descriptor->mpeg_audio_object_type()
               << std::endl;

            ss << indent << "frequency_index: "
               << (uint32_t)decoder_specific_info_descriptor->frequency_index()
               << std::endl;

            ss << indent << "channel_configuration: "
               << (uint32_t)decoder_specific_info_descriptor->channel_configuration()
               << std::endl;
        }

        return ss.str();
    }

    descriptor_type descriptor() const
    {
        return m_descriptor;
    }

private:

    descriptor_type m_descriptor;
};
}
}
