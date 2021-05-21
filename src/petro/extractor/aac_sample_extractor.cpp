// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "aac_sample_extractor.hpp"

#include "aac_track_layer.hpp"
#include "adts_writer_layer.hpp"
#include "data_layer.hpp"
#include "looper_layer.hpp"
#include "media_duration_layer.hpp"
#include "parser_layer.hpp"
#include "sample_extractor_layer.hpp"
#include "timestamp_extractor_layer.hpp"
#include "track_layer.hpp"

namespace petro
{
namespace extractor
{
/// stack for extracting avc samples
struct aac_sample_extractor::impl
    : adts_writer_layer<aac_track_layer<
          looper_layer<timestamp_extractor_layer<sample_extractor_layer<
              track_layer<media_duration_layer<parser_layer<data_layer>>>>>>>>
{
};

aac_sample_extractor::aac_sample_extractor()
{
}

aac_sample_extractor::~aac_sample_extractor()
{
}

void aac_sample_extractor::open(const uint8_t* data, uint64_t size,
                                uint32_t track_id, std::error_code& error)
{
    assert(!error);
    auto impl = std::unique_ptr<aac_sample_extractor::impl>(
        new aac_sample_extractor::impl());
    impl->set_buffer(data, size);
    impl->set_track_id(track_id);
    impl->open(error);
    if (!error)
        m_impl = std::move(impl);
}

void aac_sample_extractor::close()
{
    assert(m_impl);
    m_impl->close();
    m_impl.reset();
}

void aac_sample_extractor::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

uint32_t aac_sample_extractor::track_id() const
{
    assert(m_impl);
    return m_impl->track_id();
}

uint64_t aac_sample_extractor::media_duration() const
{
    assert(m_impl);
    return m_impl->media_duration();
}

uint64_t aac_sample_extractor::decoding_timestamp() const
{
    assert(m_impl);
    return m_impl->decoding_timestamp();
}

uint64_t aac_sample_extractor::presentation_timestamp() const
{
    assert(m_impl);
    return m_impl->presentation_timestamp();
}

void aac_sample_extractor::advance()
{
    assert(m_impl);
    return m_impl->advance();
}

bool aac_sample_extractor::at_end() const
{
    assert(m_impl);
    return m_impl->at_end();
}

const uint8_t* aac_sample_extractor::sample_data() const
{
    assert(m_impl);
    return m_impl->sample_data();
}

uint32_t aac_sample_extractor::sample_size() const
{
    assert(m_impl);
    return m_impl->sample_size();
}

uint32_t aac_sample_extractor::sample_index() const
{
    assert(m_impl);
    return m_impl->sample_index();
}

uint32_t aac_sample_extractor::samples() const
{
    assert(m_impl);
    return m_impl->samples();
}

void aac_sample_extractor::write_adts_header(uint8_t* data) const
{
    assert(m_impl);
    m_impl->write_adts_header(data);
}

uint32_t aac_sample_extractor::adts_header_size() const
{
    assert(m_impl);
    return m_impl->adts_header_size();
}

uint8_t aac_sample_extractor::mpeg_audio_object_type() const
{
    assert(m_impl);
    return m_impl->mpeg_audio_object_type();
}

uint32_t aac_sample_extractor::frequency_index() const
{
    assert(m_impl);
    return m_impl->frequency_index();
}

uint8_t aac_sample_extractor::channel_configuration() const
{
    assert(m_impl);
    return m_impl->channel_configuration();
}

void aac_sample_extractor::enable_looping()
{
    assert(m_impl);
    return m_impl->enable_looping();
}

void aac_sample_extractor::disable_looping()
{
    assert(m_impl);
    return m_impl->disable_looping();
}

uint32_t aac_sample_extractor::loops() const
{
    assert(m_impl);
    return m_impl->loops();
}

}
}
