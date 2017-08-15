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
/// binary XML container
class bxml : public data_box
{

public:

    static const std::string TYPE;

public:

    bxml(const uint8_t* data, uint64_t size) :
        data_box(data, size)
    { }

    error box_error_code() const override
    {
        return error::invalid_bxml_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
