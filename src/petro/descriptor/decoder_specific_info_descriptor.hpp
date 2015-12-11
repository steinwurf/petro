// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>

#include "descriptor.hpp"

#include "../byte_stream.hpp"
#include "../bit_reader.hpp"

namespace petro
{
namespace descriptor
{
    class decoder_specific_info_descriptor : public descriptor
    {
    public:
        decoder_specific_info_descriptor(byte_stream& bs, uint8_t tag):
            descriptor(bs, tag)
        {
            assert(m_tag == 0x05);
            while (m_remaining_bytes != 0)
            {
                m_specific_info.push_back(bs.read_uint8_t());
                m_remaining_bytes -= 1;
            }

            auto bit_reader = petro::bit_reader(m_specific_info);
            m_mpeg_audio_object_type = bit_reader.read_5_bits();

            if (m_mpeg_audio_object_type == 0x1f)
                m_mpeg_audio_object_type = 32 + bit_reader.read_6_bits();

            m_frequency_index = bit_reader.read_4_bits();

            if (m_frequency_index == 15)
                m_frequency_index =
                   (uint32_t) bit_reader.read_8_bits() << 16 |
                   (uint32_t) bit_reader.read_8_bits() << 8 |
                   (uint32_t) bit_reader.read_8_bits();

            m_channel_configuration = bit_reader.read_4_bits();
        }
        std::vector<uint8_t> specific_info() const
        {
            return m_specific_info;
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

        std::vector<uint8_t> m_specific_info;
        uint8_t m_mpeg_audio_object_type;
        uint32_t m_frequency_index;
        uint8_t m_channel_configuration;
    };
}
}
