// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#include "../parser.hpp"
#include "../box/all.hpp"
#include "../descriptor/decoder_config_descriptor.hpp"
#include "mpeg_versions.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
namespace petro
{
namespace extractor
{
    class aac_extractor
    {
    public:
        aac_extractor(std::ifstream& file);

        bool has_next_sample();
        std::vector<char> next_sample();

    private:
        std::vector<uint8_t> create_adts(
            uint16_t aac_frame_length,
            uint8_t channel_configuration,
            uint8_t frequency_index,
            uint8_t mpeg_audio_object_type,
            mpeg_versions mpeg_version = mpeg_versions::version4,
            uint8_t number_of_raw_data_blocks = 1);

    private:
        std::ifstream& m_file;

        uint32_t m_found_samples = 0;
        uint32_t m_index = 0;
        uint32_t m_j = 0;
        std::shared_ptr<const box::stco> m_stco;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
        uint8_t m_channel_configuration;
        uint32_t m_frequency_index;
        uint8_t m_mpeg_audio_object_type;
    };
}
}
