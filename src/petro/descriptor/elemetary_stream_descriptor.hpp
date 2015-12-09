// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "descriptor.hpp"
#include "decoder_config_descriptor.hpp"
#include "sl_config_descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace descriptor
{
    class elemetary_stream_descriptor : public descriptor
    {
    public:

        using decoder_config_descriptor_type =
            petro::descriptor::decoder_config_descriptor;

    public:
        elemetary_stream_descriptor(byte_stream& bs, uint8_t tag):
            descriptor(bs, tag)
        {
            assert(m_tag == 0x03);
            m_es_id = bs.read_uint16_t();
            m_remaining_bytes -= 2;

            auto d = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            auto stream_dependence_flag = (d & 0x01) == 1;
            auto url_flag = ((d & 0x02) >> 1) == 1;
            auto ocr_stream_flag = ((d & 0x04) >> 2) == 1;

            m_byte_stream_priority = (d & 0xF8) >> 3;

            if (stream_dependence_flag)
            {
                m_depends_on_esid = bs.read_uint16_t();
                m_remaining_bytes -= 2;
            }

            if (url_flag)
            {
                auto url_length = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                for (uint8_t i = 0; i < url_length; ++i)
                {
                    m_url += bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }
            }

            if (ocr_stream_flag)
            {
                m_ocr_es_id = bs.read_uint16_t();
                m_remaining_bytes -= 2;
            }

            auto decoder_config_descriptor_tag = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_decoder_config_descriptor =
                std::make_shared<decoder_config_descriptor_type>(
                    bs, decoder_config_descriptor_tag);
            m_remaining_bytes -= m_decoder_config_descriptor->size();

            auto sl_config_descriptor_tag = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_sl_config_descriptor =
                std::make_shared<sl_config_descriptor>(
                    bs, sl_config_descriptor_tag);
            m_remaining_bytes -= m_sl_config_descriptor->size();

            bs.skip(m_remaining_bytes);
        }

        std::shared_ptr<decoder_config_descriptor_type>
            decoder_config_descriptor() const
        {
            return m_decoder_config_descriptor;
        }

    private:

        uint16_t m_es_id;
        uint8_t m_byte_stream_priority;
        uint16_t m_depends_on_esid;
        std::string m_url;
        uint16_t m_ocr_es_id;

        std::shared_ptr<decoder_config_descriptor_type>
            m_decoder_config_descriptor;

        std::shared_ptr<descriptor> m_sl_config_descriptor;

        // std::shared_ptr<descriptor> m_ipi_decriptor; // 0x09
        // std::vector<std::shared_ptr<descriptor>> m_content_id_descriptor; // 0x07
        // std::vector<std::shared_ptr<descriptor>> m_ipmp_descriptor; // 0x0A
        // std::vector<std::shared_ptr<descriptor>> langDescr; // 0x43
        // std::shared_ptr<descriptor> qosDescr; // 0x0C
        // std::shared_ptr<descriptor> regDescr; // 0x0D
        // std::vector<std::shared_ptr<descriptor>> extDescr; // 0x80
    };
}
}
