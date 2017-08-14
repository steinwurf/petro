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
/// movie extends header box
class mehd : public box
{

public:

    static const std::string TYPE;

public:
    mehd(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    error box_error_code() const override
    {
        return error::invalid_mehd_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
