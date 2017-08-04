// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// item information
class iinf : public box
{

public:

    static const std::string TYPE;

public:
    iinf(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }
};
}
}
