// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#include "../parser.hpp"
#include "../box/all.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
namespace petro
{
    // supported mpeg versions (?)
    uint8_t MPEG_4_VERSION = 0;
    uint8_t MPEG_2_VERSION = 1;
    class acc_extractor
    {
    public:
        acc_extractor(std::ifstream& file);

    private:
        std::vector<uint8_t> create_adts(
            uint16_t aac_frame_length,
            uint8_t channel_configuration,
            uint8_t frequency_index,
            uint8_t mpeg_audio_object_type,
            uint8_t mpeg_version = MPEG_4_VERSION,
            uint8_t number_of_raw_data_blocks = 1);

    private:
        std::ifstream& m_file;
    };
}
