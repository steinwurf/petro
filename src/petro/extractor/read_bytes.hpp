// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>

namespace petro
{
namespace extractor
{
namespace detail
{
/// Read bytes.
inline uint32_t read_bytes(uint8_t bytes, const uint8_t* data)
{
    assert(bytes != 0 && bytes <= 4);
    uint32_t result = 0;
    for (uint8_t i = 0; i < bytes; ++i)
    {
        result |= ((uint32_t)data[i]) << ((bytes - 1) - i) * 8;
    }
    return result;
}
}
}
}
