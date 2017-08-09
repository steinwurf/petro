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
/// urn
class urn : public box
{

public:

    static const std::string TYPE;

public:
    urn(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
