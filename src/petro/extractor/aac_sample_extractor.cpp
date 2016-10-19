// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "aac_sample_extractor.hpp"

#include "timestamp_extractor_layer.hpp"
#include "adts_writer_layer.hpp"
#include "sample_extractor_layer.hpp"
#include "aac_track_layer.hpp"
#include "parser_layer.hpp"
#include "memory_mapped_file_layer.hpp"

namespace petro
{
namespace extractor
{
/// stack for extracting avc samples
struct aac_sample_extractor::impl :
    timestamp_extractor_layer<
    adts_writer_layer<
    sample_extractor_layer<
    aac_track_layer<
    parser_layer<
    memory_mapped_file_layer>>>>>
{ };

aac_sample_extractor::aac_sample_extractor() :
    m_impl(new aac_sample_extractor::impl())
{ }

aac_sample_extractor::~aac_sample_extractor()
{ }

bool aac_sample_extractor::open()
{
    assert(m_impl);
    return m_impl->open();
}

void aac_sample_extractor::close()
{
    assert(m_impl);
    return m_impl->close();
}

void aac_sample_extractor::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

void aac_sample_extractor::set_file_path(const std::string& file_path)
{
    assert(m_impl);
    return m_impl->set_file_path(file_path);
}

std::string aac_sample_extractor::file_path() const
{
    assert(m_impl);
    return m_impl->file_path();
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
}
}
