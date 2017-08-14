// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "data_box.hpp"

namespace petro
{
namespace box
{
/// item location
class iloc : public data_box
{

public:

    static const std::string TYPE;

public:
    iloc(const uint8_t* data, uint64_t size) :
        data_box(data, size)
    { }

    error box_error_code() const override
    {
        return error::invalid_iloc_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
