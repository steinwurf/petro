// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "aac_sample_extractor.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>

#include "../parser.hpp"
#include "../box/moov.hpp"
#include "../box/trak.hpp"
#include "../box/mdia.hpp"
#include "../box/hdlr.hpp"
#include "../box/mdhd.hpp"
#include "../box/minf.hpp"
#include "../box/stbl.hpp"
#include "../box/stco.hpp"
#include "../box/stsc.hpp"
#include "../box/stsd.hpp"
#include "../box/co64.hpp"
#include "../box/stts.hpp"
#include "../box/stsz.hpp"
#include "../box/root.hpp"

#include "../decoding_time.hpp"

namespace petro
{
namespace extractor
{
    bool aac_sample_extractor::open(const std::string& filename)
    {
        assert(!m_file.is_open() && "Close the extractor before reopening.");
        m_file.open(filename);
        if (!m_file.is_open())
            return false;

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
        byte_stream bs((uint8_t*)m_file.data(), m_file.size());

        parser.read(root, bs);

        // Get the needed boxes
        auto mp4a = root->get_child("mp4a");
        if (mp4a == nullptr)
             return false;

        auto esds = mp4a->get_child<box::esds>();
        if (esds == nullptr)
            return false;

        auto decoder_config = esds->descriptor()->decoder_config_descriptor();

        m_channel_configuration = decoder_config->channel_configuration();
        m_frequency_index = decoder_config->frequency_index();
        m_mpeg_audio_object_type = decoder_config->mpeg_audio_object_type();

        auto trak = mp4a->get_parent("trak");
        if (trak == nullptr)
            return false;

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
            if (co64 == nullptr)
                return false;

            m_chunk_offsets.resize(co64->entry_count());
            std::copy(co64->entries().begin(), co64->entries().end(),
                      m_chunk_offsets.begin());
        }

        m_stsc = trak->get_child<box::stsc>();
        if (m_stsc == nullptr)
        {
            close();
            return false;
        }

        m_stsz = trak->get_child<box::stsz>();
        if (m_stsz == nullptr)
        {
            close();
            return false;
        }

        auto mdhd = trak->get_child<box::mdhd>();
        if (mdhd == nullptr)
        {
            close();
            return false;
        }
        m_timescale = mdhd->timescale();

        m_stts = trak->get_child<box::stts>();
        if (m_stts == nullptr)
        {
            close();
            return false;
        }

        reset();

        return true;
    }

    void aac_sample_extractor::close()
    {
        m_file.close();
        m_stsc.reset();
        m_stsz.reset();
        m_stts.reset();
        m_chunk_offsets.clear();
        m_timescale = 0;
        reset();
    }

    void aac_sample_extractor::advance()
    {
        m_sample += 1;
        m_chunk_sample += 1;
        if (m_chunk_sample >= m_stsc->samples_for_chunk(m_chunk_index))
        {
            m_chunk_index += 1;
            m_chunk_sample = 0;
        }
    }

    bool aac_sample_extractor::is_open() const
    {
        return m_file.is_open();
    }

    bool aac_sample_extractor::at_end() const
    {
        assert(m_stsz != nullptr);
        return (m_sample >= m_stsz->sample_count());
    }

    void aac_sample_extractor::reset()
    {
        m_sample = 0;
        m_chunk_index = 0;
        m_chunk_sample = 0;
    }

    const uint8_t* aac_sample_extractor::sample_data() const
    {
        assert(m_file.is_open());
        assert(m_chunk_offsets.size() != 0);
        return ((uint8_t*)m_file.data()) +  m_chunk_offsets[m_chunk_index];
    }

    uint32_t aac_sample_extractor::sample_size() const
    {
        assert(m_stsz != nullptr);
        return m_stsz->sample_size(m_sample);
    }

    uint64_t aac_sample_extractor::timestamp() const
    {
        return decoding_timestamp();
    }

    uint64_t aac_sample_extractor::decoding_timestamp() const
    {
        assert(m_stts != nullptr);
        return decoding_time(m_stts, m_timescale, m_sample);
    }

    void aac_sample_extractor::write_adts_header(uint8_t* data) const
    {
        assert(is_open());
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

    uint32_t aac_sample_extractor::adts_header_size() const
    {
        return 7U;
    }
}
}
