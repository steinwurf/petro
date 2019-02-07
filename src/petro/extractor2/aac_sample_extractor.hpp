// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <system_error>

#include "sample_extractor.hpp"

namespace petro
{
namespace extractor
{
class aac_sample_extractor : sample_extractor
{
public:

    void open(
        const uint8_t* data,
        uint64_t size,
        uint32_t track_id,
        std::error_code& error)
    {
        sample_extractor::open(data, size, track_id, error);
    }
};
}
}
