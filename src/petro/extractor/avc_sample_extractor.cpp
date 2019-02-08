// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "avc_sample_extractor.hpp"

#include "nalu_header_writer_layer.hpp"
#include "avc_track_layer.hpp"
#include "looper_layer.hpp"
#include "timestamp_extractor_layer.hpp"
#include "sample_extractor_layer.hpp"
#include "media_duration_layer.hpp"
#include "track_layer.hpp"
#include "parser_layer.hpp"
#include "data_layer.hpp"

namespace petro
{
namespace extractor
{
/// stack for extracting avc samples
struct avc_sample_extractor::impl :
    nalu_header_writer_layer<
    avc_track_layer<
    looper_layer<
    timestamp_extractor_layer<
    sample_extractor_layer<
    track_layer<
    media_duration_layer<
    parser_layer<
    data_layer>>>>>>>>
{ };

avc_sample_extractor::avc_sample_extractor()
{ }

avc_sample_extractor::~avc_sample_extractor()
{ }

void avc_sample_extractor::open(
        const uint8_t* data,
        uint32_t size,
        uint32_t track_id,
        std::error_code& error)
{
    assert(!error);
    auto impl = std::unique_ptr<avc_sample_extractor::impl>(
        new avc_sample_extractor::impl());
    impl->set_buffer(data, size);
    impl->set_track_id(track_id);
    impl->open(error);
    if (!error)
        m_impl = std::move(impl);
}

uint32_t avc_sample_extractor::track_id() const
{
    assert(m_impl);
    return m_impl->track_id();
}

void avc_sample_extractor::close()
{
    assert(m_impl);
    m_impl->close();
    m_impl.reset();
}

void avc_sample_extractor::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

uint64_t avc_sample_extractor::media_duration() const
{
    assert(m_impl);
    return m_impl->media_duration();
}

uint64_t avc_sample_extractor::decoding_timestamp() const
{
    assert(m_impl);
    return m_impl->decoding_timestamp();
}

uint64_t avc_sample_extractor::presentation_timestamp() const
{
    assert(m_impl);
    return m_impl->presentation_timestamp();
}

void avc_sample_extractor::advance()
{
    assert(m_impl);
    return m_impl->advance();
}

bool avc_sample_extractor::at_end() const
{
    assert(m_impl);
    return m_impl->at_end();
}

const uint8_t* avc_sample_extractor::sample_data() const
{
    assert(m_impl);
    return m_impl->sample_data();
}

uint32_t avc_sample_extractor::sample_size() const
{
    assert(m_impl);
    return m_impl->sample_size();
}

uint32_t avc_sample_extractor::sample_index() const
{
    assert(m_impl);
    return m_impl->sample_index();
}

uint32_t avc_sample_extractor::samples() const
{
    assert(m_impl);
    return m_impl->samples();
}

const uint8_t* avc_sample_extractor::pps_data() const
{
    assert(m_impl);
    return m_impl->pps_data();
}

uint32_t avc_sample_extractor::pps_size() const
{
    assert(m_impl);
    return m_impl->pps_size();
}

const uint8_t* avc_sample_extractor::sps_data() const
{
    assert(m_impl);
    return m_impl->sps_data();
}

uint32_t avc_sample_extractor::sps_size() const
{
    assert(m_impl);
    return m_impl->sps_size();
}

uint8_t avc_sample_extractor::nalu_length_size() const
{
    assert(m_impl);
    return m_impl->nalu_length_size();
}

/// Write the nalu header to the given data pointer
void avc_sample_extractor::write_nalu_header(uint8_t* data) const
{
    assert(m_impl);
    return m_impl->write_nalu_header(data);
}

/// Returns the size of the nalu header
uint32_t avc_sample_extractor::nalu_header_size() const
{
    assert(m_impl);
    return m_impl->nalu_header_size();
}

void avc_sample_extractor::enable_looping()
{
    assert(m_impl);
    return m_impl->enable_looping();
}

void avc_sample_extractor::disable_looping()
{
    assert(m_impl);
    return m_impl->disable_looping();
}

uint32_t avc_sample_extractor::loops() const
{
    assert(m_impl);
    return m_impl->loops();
}
}
}
