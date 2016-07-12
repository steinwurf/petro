// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>

#include "../parser.hpp"
#include "../box/all.hpp"
#include "mpeg_versions.hpp"

namespace petro
{
namespace extractor
{
    class aac_extractor
    {
    public:

        aac_extractor(std::ifstream& file, bool loop = false);

        bool advance_to_next_sample();
        std::vector<uint8_t> sample_data();
        uint64_t decoding_timestamp();
        uint64_t sample_delta();

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
        std::vector<uint8_t> m_sample_data;

        uint32_t m_chunk_index;
        uint32_t m_chunk_sample;
        uint32_t m_sample;

        // The decoding timestamp is increasing monotonically for each sample
        uint64_t m_decoding_timestamp;
        // Time difference between the current and the previous sample
        uint64_t m_sample_delta;

        uint32_t m_timescale;

        // Indicates if the sample extraction should be looped
        bool m_loop;
        // The loop offset stores the timestamp upon the completion of a loop
        uint32_t m_loop_offset;

        std::vector<uint64_t> m_chunk_offsets;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
        std::shared_ptr<const box::stts> m_stts;

        uint8_t m_channel_configuration;
        uint32_t m_frequency_index;
        uint8_t m_mpeg_audio_object_type;
    };
}
}
