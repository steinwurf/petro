// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "../box/avcc.hpp"
#include "../box/ctts.hpp"
#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/stts.hpp"

namespace petro
{
namespace extractor
{
    class h264_extractor
    {
    public:

        h264_extractor(const std::string& filename, bool loop = false);

        bool advance_to_next_sample();
        std::vector<uint8_t> sample_data() const;
        uint64_t decoding_timestamp() const;
        uint64_t presentation_timestamp() const;
        uint64_t sample_delta() const;

        std::vector<uint8_t> sps() const;
        std::vector<uint8_t> pps() const;
        uint32_t video_width() const;
        uint32_t video_height() const;

    private:

        uint32_t read_nalu_size();

    private:

        std::shared_ptr<std::ifstream> m_file;
        std::vector<uint8_t> m_sample_data;

        uint32_t m_chunk_index;
        uint32_t m_chunk_sample;
        uint32_t m_sample;

        // This timestamp might *not* increase monotonically for each frame if
        // the decoding order differs from the presentation order
        uint32_t m_presentation_timestamp;
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
        std::shared_ptr<const box::avcc> m_avcc;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
        std::shared_ptr<const box::stts> m_stts;
        std::shared_ptr<const box::ctts> m_ctts;
    };
}
}
