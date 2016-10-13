// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "avc_sample_extractor.hpp"

#include "timestamp_extractor_layer.hpp"
#include "sample_extractor_layer.hpp"
#include "avc_track_layer.hpp"
#include "parser_layer.hpp"
#include "memory_mapped_file_layer.hpp"

namespace petro
{
namespace extractor
{
/// stack for extracting avc samples
struct avc_sample_extractor::impl :
    timestamp_extractor_layer<
    sample_extractor_layer<
    avc_track_layer<
    parser_layer<
    memory_mapped_file_layer>>>>
{ };

avc_sample_extractor::avc_sample_extractor() :
    m_impl(new avc_sample_extractor::impl())
{ }

avc_sample_extractor::~avc_sample_extractor()
{ }

bool avc_sample_extractor::open()
{
    assert(m_impl);
    return m_impl->open();
}

void avc_sample_extractor::close()
{
    assert(m_impl);
    return m_impl->close();
}

void avc_sample_extractor::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

void avc_sample_extractor::set_file_path(const std::string& file_path)
{
    assert(m_impl);
    return m_impl->set_file_path(file_path);
}

std::string avc_sample_extractor::file_path() const
{
    assert(m_impl);
    return m_impl->file_path();
}

/// Return the total video length in microseconds
uint64_t avc_sample_extractor::video_length() const
{
    assert(m_impl);
    return m_impl->video_length();
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

}
}
