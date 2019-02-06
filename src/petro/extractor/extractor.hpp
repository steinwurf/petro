// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <system_error>

namespace petro
{
namespace extractor
{
/// interface for extracting samples
struct extractor
{
    /// Open this an the underlying layers
    virtual void open(std::error_code& error) = 0;

    /// Close this an the underlying layers
    virtual void close() = 0;

    /// Reset the state of the extractor
    virtual void reset() = 0;

    /// Sets the file path of the fíle to open.
    virtual void set_file_path(const std::string& file_path) = 0;

    /// Returns the set file path.
    virtual std::string file_path() const = 0;

    /// Return the total media duration in microseconds
    virtual uint64_t media_duration() const = 0;

    /// Return the decoding timestamp related to the current sample
    virtual uint64_t decoding_timestamp() const = 0;

    /// Return the presentation timestamp related to the current sample
    virtual uint64_t presentation_timestamp() const = 0;

    /// Advance to next sample
    virtual void advance() = 0;

    /// Return true if no more sample are available.
    virtual bool at_end() const = 0;

    /// Return a pointer to the sample data
    virtual const uint8_t* sample_data() const = 0;

    /// Return the size of the sample data
    virtual uint32_t sample_size() const = 0;

    /// Return the current sample index
    virtual uint32_t sample_index() const = 0;

    /// Return the number of samples
    virtual uint32_t samples() const = 0;
};
}
}
