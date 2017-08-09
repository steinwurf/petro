// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"

namespace petro
{
namespace box
{
/// copyright etc.
class cprt : public box
{

public:

    static const std::string TYPE;

public:
    cprt(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
