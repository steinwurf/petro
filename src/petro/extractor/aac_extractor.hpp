// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include <boost/iostreams/device/mapped_file.hpp>

#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/stts.hpp"
#include "mpeg_versions.hpp"

namespace petro
{
namespace extractor
{
    class aac_extractor
    {
    public:

        aac_extractor(const std::string& filename, bool loop = false);

        // Set if the ADTS header should be added to the extracted samples.
        // The default setting is enabled (the ADTS headers are added).
        void use_adts_header(bool enabled);

        /// @return true if there are no more samples
        bool at_end() const;

        /// Loads the next sample from the file: the sample data and the
        /// corresponding timestamp can be retrieved with the getter functions
        /// @return true if a new sample was successfully extracted
        bool load_next_sample();

        /// @return Sample data of the current sample
        std::vector<uint8_t> sample_data() const;

        /// @return The decoding timestamp of the current sample
        uint64_t decoding_timestamp() const;

        /// @return The difference between the decoding timestamps of the
        /// current and the previous sample
        uint64_t sample_delta() const;

        uint32_t frequency_index() const;

        uint8_t channel_configuration() const;

        uint8_t mpeg_audio_object_type() const;

    private:

        uint32_t chunk_offset() const;

        std::vector<uint8_t> create_adts(
            uint16_t aac_frame_length,
            uint8_t channel_configuration,
            uint8_t frequency_index,
            uint8_t mpeg_audio_object_type,
            mpeg_versions mpeg_version = mpeg_versions::mpeg4,
            uint8_t number_of_raw_data_blocks = 1);

    private:

        boost::iostreams::mapped_file_source m_file;
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

        // True if the ADTS header should be added to the extracted samples
        bool m_use_adts_header;

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
