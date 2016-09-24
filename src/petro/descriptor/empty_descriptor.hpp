// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace descriptor
{
class empty_descriptor : public descriptor
{
public:
    empty_descriptor(byte_stream& bs, uint8_t tag) :
        descriptor(bs, tag)
    {
        bs.skip(m_remaining_bytes);
    }
};
}
}
