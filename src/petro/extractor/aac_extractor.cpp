// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the aacompanying LICENSE.rst file.

#include "aac_extractor.hpp"

#include <cassert>
#include <fstream>

#include "../parser.hpp"
#include "../box/all.hpp"
#include "../decoding_time.hpp"
#include "../descriptor/decoder_config_descriptor.hpp"

namespace petro
{
namespace extractor
{
    aac_extractor::aac_extractor(const std::string& filename, bool loop) :
        m_file(std::make_shared<std::ifstream>(filename, std::ios::binary)),
        m_chunk_index(0),
        m_chunk_sample(0),
        m_sample(0),
        m_decoding_timestamp(0),
        m_sample_delta(0),
        m_loop(loop),
        m_loop_offset(0),
        m_use_adts_header(true)
    {
        assert(m_file->is_open() && "Cannot open input file");
        assert(m_file->good() && "Invalid input file");

        parser<
            box::moov<parser<
                box::trak<parser<
                    box::mdia<parser<
                        box::hdlr,
                        box::mdhd,
                        box::minf<parser<
                            box::stbl<parser<
                                box::stco,
                                box::stsc,
                                box::stsd,
                                box::co64,
                                box::stts,
                                box::stsz
                            >>
                        >>
                    >>
                >>
            >>
        > parser;

        auto root = std::make_shared<box::root>();
        byte_stream bs(*m_file);

        parser.read(root, bs);

        // get needed boxes
        auto mp4a = root->get_child("mp4a");
        assert(mp4a != nullptr);

        auto esds = mp4a->get_child<box::esds>();
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

        auto stco = trak->get_child<box::stco>();
        if (stco != nullptr)
        {
            m_chunk_offsets.resize(stco->entry_count());
            std::copy(stco->entries().begin(), stco->entries().end(),
                      m_chunk_offsets.begin());
        }
        else
        {
            auto co64 = trak->get_child<box::co64>();
            assert(co64 != nullptr);
            m_chunk_offsets.resize(co64->entry_count());
            std::copy(co64->entries().begin(), co64->entries().end(),
                      m_chunk_offsets.begin());
        }

        m_stsc = trak->get_child<box::stsc>();
        assert(m_stsc != nullptr);

        m_stsz = trak->get_child<box::stsz>();
        assert(m_stsz != nullptr);

        auto mdhd = trak->get_child<box::mdhd>();
        assert(mdhd != nullptr);
        m_timescale = mdhd->timescale();

        m_stts = trak->get_child<box::stts>();
        assert(m_stts != nullptr);

        // Seek to the first chunk in the file
        m_file->seekg(m_chunk_offsets[m_chunk_index]);
    }

    void aac_extractor::use_adts_header(bool enabled)
    {
        m_use_adts_header = enabled;
    }

    bool aac_extractor::at_end() const
    {
        return (m_sample >= m_stsz->sample_count());
    }

    bool aac_extractor::load_next_sample()
    {
        if (at_end())
            return false;

        uint32_t sample_size = m_stsz->sample_size(m_sample);

        if (m_use_adts_header)
        {
            // Create the ADTS header
            m_sample_data = create_adts(
                sample_size,
                m_channel_configuration,
                m_frequency_index,
                m_mpeg_audio_object_type);

            // Append the sample data to the header
            uint32_t header_size = m_sample_data.size();
            m_sample_data.resize(header_size + sample_size);
            m_file->read((char*)&m_sample_data[header_size], sample_size);
        }
        else
        {
            m_sample_data.resize(sample_size);
            m_file->read((char*)m_sample_data.data(), sample_size);
        }

        uint64_t decoding_timestamp =
            decoding_time(m_stts, m_timescale, m_sample) +
            m_loop_offset;

        m_sample_delta = decoding_timestamp - m_decoding_timestamp;
        m_decoding_timestamp = decoding_timestamp;
        m_sample++;

        m_chunk_sample += 1;
        if (m_chunk_sample >= m_stsc->samples_for_chunk(m_chunk_index))
        {
            m_chunk_index += 1;
            m_chunk_sample = 0;
            if (m_chunk_index >= m_chunk_offsets.size())
            {
                // We processed the last chunk at this point
                if (m_loop)
                {
                    m_sample = 0;
                    m_chunk_index = 0;
                    m_loop_offset = m_decoding_timestamp;
                    m_file->seekg(m_chunk_offsets[m_chunk_index]);
                }
            }
            else
            {
                m_file->seekg(m_chunk_offsets[m_chunk_index]);
            }
        }

        return true;
    }

    std::vector<uint8_t> aac_extractor::sample_data() const
    {
        return m_sample_data;
    }

    uint64_t aac_extractor::decoding_timestamp() const
    {
        return m_decoding_timestamp;
    }

    uint64_t aac_extractor::sample_delta() const
    {
        return m_sample_delta;
    }

    uint32_t aac_extractor::frequency_index() const
    {
        return m_frequency_index;
    }

    uint8_t aac_extractor::channel_configuration() const
    {
        return m_channel_configuration;
    }

    uint8_t aac_extractor::mpeg_audio_object_type() const
    {
        return m_mpeg_audio_object_type;
    }

    /// Creates an Audio Data Transport Stream (ADTS) header which is to be
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
    /// G      1    private bit, guaranteed never to be used by MPEG, set to 0
    ///             when encoding, ignore when decoding
    /// H      3    MPEG-4 Channel Configuration (in the case of 0, the channel
    ///             configuration is sent via an inband PCE)
    /// I      1    originality, set to 0 when encoding, ignore when decoding
    /// J      1    home, set to 0 when encoding, ignore when decoding
    /// K      1    copyrighted id bit, the next bit of a centrally registered
    ///             copyright identifier, set to 0 when encoding, ignore when
    ///             decoding.
    /// L      1    copyright id start, signals that this frame's copyright id
    ///             bit is the first bit of the copyright id, set to 0 when
    ///             encoding, ignore when decoding.
    /// M      13   frame length, this value must include 7 or 9 bytes of header
    ///             length: FrameLength =
    ///             (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    /// O      11   Buffer fullness, can be ignored when decoding.
    /// P      2    Number of AAC frames (raw data blocks) in ADTS frame
    ///             minus 1, for maximum compatibility always use 1 AAC frame
    ///             per ADTS frame.
    /// Q      16   CRC if protection absent is 0 (not shown in structure)
    ///
    /// Source: http://wiki.multimedia.cx/index.php?title=ADTS
    ///
    std::vector<uint8_t> aac_extractor::create_adts(
        uint16_t aac_frame_length,
        uint8_t channel_configuration,
        uint8_t frequency_index,
        uint8_t mpeg_audio_object_type,
        mpeg_versions mpeg_version,
        uint8_t number_of_raw_data_blocks)
    {
        assert(mpeg_version == mpeg_versions::mpeg4 ||
               mpeg_version == mpeg_versions::mpeg2);
        uint8_t protection_absent = 1;

        std::vector<uint8_t> adts;

        uint8_t byte1 = 0xFF;
        adts.push_back(byte1);

        uint8_t byte2 = 0xF0;
        byte2 |= static_cast<uint8_t>(mpeg_version) << 3;
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
