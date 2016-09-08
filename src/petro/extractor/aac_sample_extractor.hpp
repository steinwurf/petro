// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "../box/avcc.hpp"
#include "../box/ctts.hpp"
#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/stts.hpp"
#include "mpeg_versions.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

namespace petro
{
namespace extractor
{
    class aac_sample_extractor
    {
    public:

        bool open(const std::string& filename);

        void close();

        void advance();

        bool is_open() const;
        bool at_end() const;

        void reset();

        const uint8_t* sample_data() const;

        uint32_t sample_size() const;

        uint64_t timestamp() const;

        uint64_t decoding_timestamp() const;

        /// Writes an Audio Data Transport Stream (ADTS) header which is to be
        /// placed before every AAC sample.
        /// Header consists of 7 bytes.
        ///
        /// Structure
        /// AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP
        ///
        /// Letter Bits Description
        /// A      12   syncword 0xFFF, all bits must be 1
        /// B      1    MPEG Version: 0 for MPEG-4, 1 for MPEG-2
        /// C      2    Layer: always 0
        /// D      1    protection absent, Warning, set to 1 if there is no CRC
        ///             and 0 if there is CRC
        /// E      2    profile, the MPEG-4 Audio Object Type minus 1
        /// F      4    MPEG-4 Sampling Frequency Index (15 is forbidden)
        /// G      1    private bit, guaranteed never to be used by MPEG,
        ///             set to 0 when encoding, ignore when decoding
        /// H      3    MPEG-4 Channel Configuration (in the case of 0,
        ///             the channel configuration is sent via an inband PCE)
        /// I      1    originality, set to 0 when encoding, ignore when
        ///             decoding
        /// J      1    home, set to 0 when encoding, ignore when decoding
        /// K      1    copyrighted id bit, the next bit of a centrally
        ///             registered copyright identifier, set to 0 when encoding,
        ///             ignore when decoding.
        /// L      1    copyright id start, signals that this frame's copyright
        ///             id bit is the first bit of the copyright id, set to 0
        ///             when encoding, ignore when decoding.
        /// M      13   frame length, this value must include 7 or 9 bytes of
        ///             header length: FrameLength =
        ///             (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
        /// O      11   Buffer fullness, can be ignored when decoding.
        /// P      2    Number of AAC frames (raw data blocks) in ADTS frame
        ///             minus 1, for maximum compatibility always use 1 AAC
        ///             frame per ADTS frame.
        /// Q      16   CRC if protection absent is 0 (not shown in structure)
        ///
        /// Source: http://wiki.multimedia.cx/index.php?title=ADTS
        void write_adts_header(uint8_t* data) const;

        uint32_t adts_header_size() const;

    private:

        boost::iostreams::mapped_file_source m_file;

        uint32_t m_sample = 0;
        uint32_t m_chunk_index = 0;
        uint32_t m_chunk_sample = 0;
        uint32_t m_timescale = 0;

        mpeg_versions m_mpeg_version = mpeg_versions::mpeg4;
        uint8_t m_protection_absent = 1;

        uint8_t m_mpeg_audio_object_type = 0;
        uint32_t m_frequency_index = 0;
        uint8_t m_channel_configuration = 0;
        uint8_t m_number_of_raw_data_blocks = 1;

        std::vector<uint64_t> m_chunk_offsets;

        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
        std::shared_ptr<const box::stts> m_stts;

    };
}
}
