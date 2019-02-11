// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <cassert>
#include <vector>
#include "avc_to_annex_b.hpp"

namespace petro
{
namespace extractor
{
namespace detail
{
/// Read bytes.
uint32_t read_bytes(uint8_t bytes, const uint8_t* data)
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

/// Updates the state of this layer.
std::vector<std::pair<const uint8_t*, uint64_t>> avc_to_annex_b(
    const uint8_t* data, uint64_t size, const uint32_t nalu_length_size)
{
    std::vector<std::pair<const uint8_t*, uint64_t>> result;
    while (size != 0)
    {
        std::pair<const uint8_t*, uint64_t> nalu;
        auto nalu_size = detail::read_bytes(nalu_length_size, data);
        data += nalu_length_size;
        size -= nalu_length_size;
        nalu.first = data;
        data += nalu_size;
        size -= nalu_size;
        nalu.second = nalu_size;
        result.push_back(nalu);
    }
    return result;
}
}
}
