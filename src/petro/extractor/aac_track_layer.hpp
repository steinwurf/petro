// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include "../box/box.hpp"
#include "../box/esds.hpp"

namespace petro
{
namespace extractor
{
    template<class Super>
    class aac_track_layer : public Super
    {
    public:

        bool open(const std::string& filename)
        {
            if (!Super::open(filename))
            {
                Super::close();
                return false;
            }

            auto root = Super::root();

            auto mp4a = root->get_child("mp4a");
            if (mp4a == nullptr)
            {
                Super::close();
                return false;
            }

            auto trak = mp4a->get_parent("trak");
            if (trak == nullptr)
            {
                Super::close();
                return false;
            }

            auto esds = mp4a->get_child<box::esds>();
            if (esds == nullptr)
            {
                Super::close();
                return false;
            }

            m_trak = trak;

            auto descriptor = esds->descriptor()->decoder_config_descriptor();
            m_mpeg_audio_object_type = descriptor->mpeg_audio_object_type();
            m_frequency_index = descriptor->frequency_index();
            m_channel_configuration = descriptor->channel_configuration();

            return true;
        }

        void close()
        {
            m_trak.reset();
            uint8_t m_mpeg_audio_object_type = 0;
            uint32_t m_frequency_index = 0;
            uint8_t m_channel_configuration = 0;
        }

        void write_adts_header(uint8_t* data) const
        {
            assert(m_trak != nullptr);

            // unchangeable
            uint8_t m_number_of_raw_data_blocks = 1;
            mpeg_versions m_mpeg_version = mpeg_versions::mpeg4;
            uint8_t m_protection_absent = 1;

            // start writing
            uint32_t offset = 0;

            uint8_t byte1 = 0xFF;
            data[offset++] = byte1;

            uint8_t byte2 = 0xF0;
            byte2 |= static_cast<uint8_t>(m_mpeg_version) << 3;
            byte2 |= m_protection_absent << 0;
            data[offset++] = byte2;

            uint8_t byte3 = 0x00;
            byte3 |= (m_mpeg_audio_object_type - 1) << 6;
            byte3 |= (m_frequency_index) << 2;
            byte3 |= (m_channel_configuration & 0x04) >> 2;
            data[offset++] = byte3;

            uint8_t byte4 = 0;
            byte4 |= (m_channel_configuration & 0x03) << 6;
            // frame length, this value must include the 7 bytes of header length
            uint16_t frame_length = sample_size() + 7;
            assert(frame_length <= 0x1FFF);
            byte4 |= (frame_length & 0x1800) >> 11;
            data[offset++] = byte4;

            uint8_t byte5 = (frame_length & 0x07F8) >> 3;
            data[offset++] = byte5;

            uint8_t byte6 = 0xFF;
            byte6 &= (frame_length & 0x0007) << 5;
            data[offset++] = byte6;

            uint8_t byte7 = 0xB0;
            byte7 |= (m_number_of_raw_data_blocks - 1) & 0x03;
            data[offset++] = byte7;
        }

        uint32_t adts_header_size() const
        {
            return 7U;
        }

        std::shared_ptr<const box::box> trak() const
        {
            assert(m_trak != nullptr);
            return m_trak;
        }

    private:

        std::shared_ptr<box::box> m_trak;

        uint8_t m_mpeg_audio_object_type = 0;
        uint32_t m_frequency_index = 0;
        uint8_t m_channel_configuration = 0;
    };
}
}
