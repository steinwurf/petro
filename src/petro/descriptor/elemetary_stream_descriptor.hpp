// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "tag.hpp"
#include "descriptor.hpp"
#include "decoder_config_descriptor.hpp"

namespace petro
{
namespace descriptor
{
class elemetary_stream_descriptor : public descriptor
{
public:

    using decoder_config_descriptor_type =
        std::shared_ptr<petro::descriptor::decoder_config_descriptor>;

public:

    elemetary_stream_descriptor(const uint8_t* data, uint64_t size) :
        descriptor(data, size)
    { }

    void parse_descriptor_content(std::error_code& error) override
    {
        assert(!error);
        if (m_tag != tag::elemetary_stream)
        {
            error = std::make_error_code(std::errc::not_supported);
            return;
        }

        m_bs.read<endian::u16>(m_es_id, error);
        if (error)
            return;

        uint8_t flags = 0;
        m_bs.read<endian::u8>(flags, error);
        if (error)
            return;

        auto stream_dependence_flag = (flags & 0x01) == 1;
        auto url_flag = ((flags & 0x02) >> 1) == 1;
        auto ocr_stream_flag = ((flags & 0x04) >> 2) == 1;

        m_byte_stream_priority = (flags & 0xF8) >> 3;

        if (stream_dependence_flag)
        {
            m_bs.read<endian::u16>(m_depends_on_esid, error);
            if (error)
                return;
        }

        if (url_flag)
        {
            uint8_t url_length = 0;
            m_bs.read<endian::u8>(url_length, error);
            if (error)
                return;
            m_bs.read(m_url, url_length, error);
            if (error)
                return;
        }

        if (ocr_stream_flag)
        {
            m_bs.read<endian::u16>(m_ocr_es_id, error);
            if (error)
                return;
        }

        m_decoder_config_descriptor =
            std::make_shared<decoder_config_descriptor_type::element_type>(
                m_bs.remaining_data(), m_bs.remaining_size());
        m_decoder_config_descriptor->parse(error);
        if (error)
            return;
        m_bs.skip(m_decoder_config_descriptor->size(), error);
        if (error)
            return;

        if (m_bs.remaining_size() == 0)
            return;

        m_sl_config_descriptor =
            std::make_shared<descriptor>(
                m_bs.remaining_data(), m_bs.remaining_size());
        m_sl_config_descriptor->parse(error);
        if (error)
            return;
        m_bs.skip(m_sl_config_descriptor->size(), error);
        if (error)
            return;
        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    decoder_config_descriptor_type decoder_config_descriptor() const
    {
        return m_decoder_config_descriptor;
    }

    std::shared_ptr<descriptor> sl_config_descriptor() const
    {
        return m_sl_config_descriptor;
    }

    uint16_t es_id() const
    {
        return m_es_id;
    }

    uint8_t byte_stream_priority() const
    {
        return m_byte_stream_priority;
    }

    uint16_t depends_on_esid() const
    {
        return m_depends_on_esid;
    }

    std::string url() const
    {
        return m_url;
    }

    uint16_t ocr_es_id() const
    {
        return m_ocr_es_id;
    }

private:

    uint16_t m_es_id = 0;
    uint8_t m_byte_stream_priority = 0;
    uint16_t m_depends_on_esid = 0;
    std::string m_url;
    uint16_t m_ocr_es_id = 0;

    decoder_config_descriptor_type m_decoder_config_descriptor;

    std::shared_ptr<descriptor> m_sl_config_descriptor;
};
}
}
