// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "esds.hpp"

#include "../descriptor/elemetary_stream_descriptor.hpp"

#include <string>
#include <memory>
#include <cstdint>

namespace petro
{
namespace box
{
const std::string esds::TYPE = "esds";

esds::esds(std::weak_ptr<box> parent) :
    full_box(esds::TYPE, parent)
{ }

void esds::read(uint64_t size, byte_stream& bs)
{
    full_box::read(size, bs);

    auto elemetary_stream_descriptor_tag = bs.read_uint8_t();
    m_remaining_bytes -= 1;

    m_descriptor =
        std::make_shared<descriptor::elemetary_stream_descriptor>(
            bs, elemetary_stream_descriptor_tag);
    m_remaining_bytes -= m_descriptor->size();
}

std::string esds::describe() const
{
    std::stringstream ss;
    std::string indent = "  ";

    ss << full_box::describe() << std::endl;
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

    ss << indent << "stream_type: "
       << (uint32_t)decoder_config_descriptor->stream_type() << std::endl;

    ss << indent << "up_stream: "
       << (decoder_config_descriptor->up_stream() ? "yes" : "no")
       << std::endl;

    ss << indent << "buffer_size_db: "
       << decoder_config_descriptor->buffer_size_db() << std::endl;

    ss << indent << "max_bitrate: "
       << decoder_config_descriptor->max_bitrate() << std::endl;

    ss << indent << "average_bitrate: "
       << decoder_config_descriptor->average_bitrate() << std::endl;

    ss << indent << "mpeg_audio_object_type: "
       << (uint32_t)decoder_config_descriptor->mpeg_audio_object_type()
       << std::endl;

    ss << indent << "frequency_index: "
       << (uint32_t)decoder_config_descriptor->frequency_index()
       << std::endl;

    ss << indent << "channel_configuration: "
       << (uint32_t)decoder_config_descriptor->channel_configuration()
       << std::endl;

    return ss.str();
}

esds::descriptor_type esds::descriptor() const
{
    return m_descriptor;
}
}
}
