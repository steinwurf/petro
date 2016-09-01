// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed  under the "BSD License". See the accompanying LICENSE.rst file.

#include "h264_extractor.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <fstream>

#include "../parser.hpp"
#include "../box/all.hpp"
#include "../decoding_time.hpp"
#include "../presentation_time.hpp"

namespace petro
{
namespace extractor
{
    h264_extractor::h264_extractor(const std::string& filename, bool loop) :
        m_file(filename),
        m_chunk_index(0),
        m_chunk_sample(0),
        m_sample(0),
        m_presentation_timestamp(0),
        m_decoding_timestamp(0),
        m_sample_delta(0),
        m_loop(loop),
        m_loop_offset(0)
    {
        assert(m_file.is_open() && "Cannot open input file");

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
                                box::ctts,
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

        //get needed boxes
        auto avc1 = root->get_child("avc1");
        assert(avc1 != nullptr);

        m_avcc = avc1->get_child<box::avcc>();
        assert(m_avcc != nullptr);

        auto trak = avc1->get_parent("trak");
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

        m_ctts = trak->get_child<box::ctts>();
    }

    std::vector<uint8_t> h264_extractor::sps() const
    {
        // The sps buffer is returned with the start code
        std::vector<uint8_t> buffer = {0, 0, 0, 1};
        auto sps = m_avcc->sequence_parameter_set(0);
        std::copy(sps->data(), sps->data() + sps->size(),
                  std::back_inserter(buffer));
        return buffer;
    }

    std::vector<uint8_t> h264_extractor::pps() const
    {
        // The pps buffer is returned with the start code
        std::vector<uint8_t> buffer = {0, 0, 0, 1};
        auto pps = m_avcc->picture_parameter_set(0);
        std::copy(pps->data(), pps->data() + pps->size(),
                  std::back_inserter(buffer));
        return buffer;
    }

    uint32_t h264_extractor::video_width() const
    {
        auto sps = m_avcc->sequence_parameter_set(0);
        return sps->width();
    }

    uint32_t h264_extractor::video_height() const
    {
        auto sps = m_avcc->sequence_parameter_set(0);
        return sps->height();
    }

    bool h264_extractor::at_end() const
    {
        return (m_sample >= m_stsz->sample_count());
    }

    bool h264_extractor::load_next_sample()
    {
        if (at_end())
            return false;

        uint32_t sample_size = m_stsz->sample_size(m_sample);
        m_sample_data.clear();

        // Read multiple NALUs
        uint32_t offset = chunk_offset();
        while ((offset - chunk_offset()) < sample_size)
        {
            uint32_t nalu_size = read_nalu_size((uint8_t*)m_file.data() + offset);
            offset += m_avcc->length_size();

            // Replace the AVCC headers with the start code,
            // so each NALU will have the 4-byte start code: 00 00 00 01
            m_sample_data.push_back(0);
            m_sample_data.push_back(0);
            m_sample_data.push_back(0);
            m_sample_data.push_back(1);
            m_sample_data.insert(
                m_sample_data.end(),
                m_file.data() + offset,
                m_file.data() + offset + nalu_size);
            offset += nalu_size;
        }

        m_presentation_timestamp =
            presentation_time(m_stts, m_ctts, m_timescale, m_sample) +
            m_loop_offset;

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
                }
            }
        }

        return true;
    }

    std::vector<uint8_t> h264_extractor::sample_data() const
    {
        return m_sample_data;
    }

    uint64_t h264_extractor::decoding_timestamp() const
    {
        return m_decoding_timestamp;
    }

    uint64_t h264_extractor::presentation_timestamp() const
    {
        return m_presentation_timestamp;
    }

    uint64_t h264_extractor::sample_delta() const
    {
        return m_sample_delta;
    }

    uint32_t h264_extractor::chunk_offset() const
    {
        return m_chunk_offsets[m_chunk_index];
    }

    uint32_t h264_extractor::read_nalu_size(const uint8_t* data) const
    {
        uint32_t result = 0;
        for (uint8_t i = 0; i < m_avcc->length_size(); ++i)
        {
            result |= data[i] << ((m_avcc->length_size() - 1) - i) * 8;
        }
        return result;
    }
}
}
