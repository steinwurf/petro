// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <memory>

namespace petro
{
namespace extractor
{

// stack for extracting avc samples
struct avc_sample_extractor
{
    /// Constructor
    avc_sample_extractor();

    /// Destructor
    ~avc_sample_extractor();

    /// Open this an the underlying layers
    bool open();

    /// Close this an the underlying layers
    void close();

    /// Reset the state of the extractor
    void reset();

    /// Sets the file path of the fíle to open.
    void set_file_path(const std::string& file_path);

    /// Returns the file path of the fíle to open.
    std::string file_path() const;

    /// Return the timestamp related to the current sample
    uint64_t timestamp() const;

    /// Return the decoding timestamp related to the current sample
    uint64_t decoding_timestamp() const;

    /// Return the presentation timestamp related to the current sample
    uint64_t presentation_timestamp() const;

    /// Advance to next sample
    void advance();

    /// Return true if no more sample are available.
    bool at_end() const;

    /// Return a pointer to the sample data
    const uint8_t* sample_data() const;

    /// Return the size of the sample data
    uint32_t sample_size() const;

    /// Return the current sample index
    uint32_t sample_index() const;

    /// Return a pointer to the pps data
    const uint8_t* pps_data() const;

    /// Return the size of the pps data
    uint32_t pps_size() const;

    /// Return a pointer to the sps data
    const uint8_t* sps_data() const;

    /// Return the size of the sps data
    uint32_t sps_size() const;

    /// Return the size of the length preceeded each nalu sample in the h264
    /// sample.
    uint8_t nalu_length_size() const;

private:

    // Private implemenation
    struct impl;
    std::unique_ptr<impl> m_impl;
};

}
}
