// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the aacompanying LICENSE.rst file.
#include "aac_extractor.hpp"

#include <fstream>
#include <string>
#include <memory>
#include <iostream>
namespace petro
{
namespace extractor
{
    aac_extractor::aac_extractor(std::ifstream& file) :
    m_file(file)
    {
        petro::parser<
            petro::box::moov<petro::parser<
                petro::box::trak<petro::parser<
                    petro::box::mdia<petro::parser<
                        petro::box::hdlr,
                        petro::box::minf<petro::parser<
                            petro::box::stbl<petro::parser<
                                petro::box::stco,
                                petro::box::stsc,
                                petro::box::stsd,
                                petro::box::stsz
                            >>
                        >>
                    >>
                >>
            >>
        > parser;

        auto root = std::make_shared<petro::box::root>();
        petro::byte_stream bs(m_file);

        parser.read(root, bs);

        // get needed boxes
        auto mp4a = root->get_child("mp4a");
        assert(mp4a != nullptr);

        auto esds = mp4a->get_child<petro::box::esds>();
        assert(esds != nullptr);
        auto decoder_config_descriptor =
            esds->descriptor()->decoder_config_descriptor();

        m_channel_configuration =
            decoder_config_descriptor->channel_configuration();
        m_frequency_index = decoder_config_descriptor->frequency_index();
        m_mpeg_audio_object_type =
            decoder_config_descriptor->mpeg_audio_object_type();

        auto trak = mp4a->get_parent("trak");
        assert(trak != nullptr);

        m_stco = trak->get_child<petro::box::stco>();
        assert(m_stco != nullptr);

        m_stsc = trak->get_child<petro::box::stsc>();
        assert(m_stsc != nullptr);

        m_stsz = trak->get_child<petro::box::stsz>();
        assert(m_stsz != nullptr);
    }

    bool aac_extractor::has_next_adts()
    {
        // If
        if(m_index == m_stco->entry_count())
        {
            return false;
        }

        if(m_index != m_stco->entry_count() - 1)
        {
            return true;
        }

        if(m_j < m_stsc->samples_for_chunk(m_index))
        {
            return true;
        }
        return false;

    }

    std::vector<char> aac_extractor::next_adts()
    {
        if(!(m_j < m_stsc->samples_for_chunk(m_index)))
        {
            m_index++;
            m_file.seekg(m_stco->chunk_offset(m_index));
            m_j = 0;
        }

        uint16_t sample_size = m_stsz->sample_size(m_found_samples);

        auto atds = create_adts(
            sample_size,
            m_channel_configuration,
            m_frequency_index,
            m_mpeg_audio_object_type);

        std::vector<char> temp(sample_size);
        m_file.read(temp.data(), sample_size);
        m_found_samples++;
        m_j++;
        return temp;
    }

    std::vector<uint8_t> aac_extractor::create_adts(
        uint16_t aac_frame_length,
        uint8_t channel_configuration,
        uint8_t frequency_index,
        uint8_t mpeg_audio_object_type,
        uint8_t mpeg_version,
        uint8_t number_of_raw_data_blocks)
    {
        assert(mpeg_version == MPEG_4_VERSION || mpeg_version == MPEG_2_VERSION);
        uint8_t protection_absent = 1;

        std::vector<uint8_t> adts;

        uint8_t byte1 = 0xFF;
        adts.push_back(byte1);

        uint8_t byte2 = 0xF0;
        byte2 |= mpeg_version << 3;
        byte2 |= protection_absent << 0;

        adts.push_back(byte2);

        uint8_t byte3 = 0x00;
        byte3 |= (mpeg_audio_object_type - 1) << 6;
        byte3 |= frequency_index << 2;

        byte3 |= (channel_configuration & 0x04) >> 2;
        adts.push_back(byte3);

        uint8_t byte4 = 0;

        byte4 |= (channel_configuration & 0x03) << 6;
        // frame length, this value must include the 7 bytes of header length
        uint16_t frame_length = aac_frame_length + 7;
        assert(frame_length <= 0x1FFF);
        byte4 |= (frame_length & 0x1800) >> 11;

        adts.push_back(byte4);

        adts.push_back((frame_length & 0x07F8) >> 3); // byte5

        uint8_t byte6 = 0xFF;
        byte6 &= (frame_length & 0x0007) << 5;
        adts.push_back(byte6);

        uint8_t byte7 = 0xB0;
        byte7 |= (number_of_raw_data_blocks - 1) & 0x03;
        adts.push_back(byte7);

        return adts;
    }
}
}
