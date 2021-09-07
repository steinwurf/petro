// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <system_error>

#include "extractor.hpp"

namespace petro
{
namespace extractor
{

/// stack for extracting avc samples
struct avc_sample_extractor : extractor
{
    /// Constructor
    ///
    /// Note: Why we need to declare and define both constructor and destructor
    /// when using std::unique_ptr and pimpl:
    /// http://stackoverflow.com/a/9954553/1717320
    avc_sample_extractor();

    /// Destructor (see constructor why definition of destructor is needed)
    ~avc_sample_extractor();

    /// Open this an the underlying layers
    void open(const uint8_t* data, uint64_t size, uint32_t track_id,
              std::error_code& error) override;

    /// Gets the track id.
    uint32_t track_id() const override;

    /// Close this an the underlying layers
    void close() override;

    /// Reset the state of the extractor
    void reset() override;

    /// Return the total media duration in microseconds
    uint64_t media_duration() const override;

    /// Return the decoding timestamp related to the current sample
    uint64_t decoding_timestamp() const override;

    /// Return the presentation timestamp related to the current sample
    uint64_t presentation_timestamp() const override;

    /// Advance to next sample
    void advance() override;

    /// Return true if no more sample are available.
    bool at_end() const override;

    /// Return a pointer to the sample data
    const uint8_t* sample_data() const override;

    /// Return the size of the sample data
    uint32_t sample_size() const override;

    /// Return the current sample index
    uint32_t sample_index() const override;

    /// Return the number of samples
    uint32_t samples() const override;

    /// Return a pointer to the pps data
    const uint8_t* pps_data() const;

    /// Return the size of the pps data
    uint64_t pps_size() const;

    /// Return a pointer to the sps data
    const uint8_t* sps_data() const;

    /// Return the size of the sps data
    uint64_t sps_size() const;

    /// Return the size of the length preceded each nalu sample in the h264
    /// sample.
    uint8_t nalu_length_size() const;

    /// Write the nalu header to the given data pointer
    void write_nalu_header(uint8_t* data) const;

    /// Returns the size of the nalu header
    uint32_t nalu_header_size() const;

    /// Enables looping
    void enable_looping() override;

    /// Disables looping
    void disable_looping() override;

    /// Returns the number of times the extractor has looped
    uint32_t loops() const override;

private:
    // Private implemenation
    struct impl;
    std::unique_ptr<impl> m_impl;
};

}
}
