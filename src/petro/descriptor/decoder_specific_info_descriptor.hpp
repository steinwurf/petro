// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "descriptor.hpp"

#include "../bit_reader.hpp"

namespace petro
{
namespace descriptor
{
class decoder_specific_info_descriptor : public descriptor
{
public:

    decoder_specific_info_descriptor(const uint8_t* data, uint64_t size) :
        descriptor(data, size)
    { }

    void parse_descriptor_content(std::error_code& error) override
    {
        assert(!error);
        if (m_tag != tag::decoder_specific_info)
        {
            error = std::make_error_code(std::errc::not_supported);
            return;
        }

        auto bit_reader =
            petro::bit_reader(m_bs.remaining_data(), m_bs.remaining_size());

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;

        m_mpeg_audio_object_type = bit_reader.read_bits(5);

        if (m_mpeg_audio_object_type == 0x1F)
            m_mpeg_audio_object_type = 32 + bit_reader.read_bits(6);

        m_frequency_index = bit_reader.read_bits(4);

        if (m_frequency_index == 0x0F)
            m_frequency_index = bit_reader.read_bits(24);

        m_channel_configuration = bit_reader.read_bits(4);
    }

    uint8_t mpeg_audio_object_type() const
    {
        return m_mpeg_audio_object_type;
    }

    uint32_t frequency_index() const
    {
        return m_frequency_index;
    }

    uint8_t channel_configuration() const
    {
        return m_channel_configuration;
    }

private:

    uint8_t m_mpeg_audio_object_type = 0;
    uint32_t m_frequency_index = 0;
    uint8_t m_channel_configuration = 0;
};
}
}
