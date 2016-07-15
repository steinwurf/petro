// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>
#include <cstdint>

#include "box/stts.hpp"

namespace petro
{
    /// Returns the sample's decoding time in microseconds.
    inline uint64_t decoding_time(
        std::shared_ptr<const box::stts> stts,
        uint32_t media_header_timescale,
        uint32_t sample_index)
    {
        uint64_t decoding_time = stts->decoding_time(sample_index);

        decoding_time *= 1000000;
        decoding_time /= media_header_timescale;

        return decoding_time;
    }
}
