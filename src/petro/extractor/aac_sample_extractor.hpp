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
struct aac_sample_extractor
{
    /// Constructor
    ///
    /// Note: Why we need to declare and define both constructor and destructor
    /// when using std::unique_ptr and pimpl:
    /// http://stackoverflow.com/a/9954553/1717320
    aac_sample_extractor();

    /// Destructor (see constructor why definition of destructor is needed)
    ~aac_sample_extractor();

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

    /// Return the total media duration in microseconds
    uint64_t media_duration() const;

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

    /// Write the adts header to the given buffer
    void write_adts_header(uint8_t* data) const;

    /// Return the size of the adts header
    uint32_t adts_header_size() const;

private:

    // Private implemenation
    struct impl;
    std::unique_ptr<impl> m_impl;
};

}
}
