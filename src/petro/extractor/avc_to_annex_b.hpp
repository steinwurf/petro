// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

namespace petro
{
namespace extractor
{
namespace detail
{
uint32_t read_bytes(uint8_t bytes, const uint8_t* data);
}
std::vector<std::pair<const uint8_t*, uint64_t>>
avc_to_annex_b(const uint8_t* data, uint64_t size,
               const uint32_t nalu_length_size);
}
}
