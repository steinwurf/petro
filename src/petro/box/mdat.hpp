// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_box.hpp"

#include <string>
#include <vector>

namespace petro
{
namespace box
{
/// media data container
class mdat : public data_box
{

public:

    static const std::string TYPE;

public:
    mdat(const uint8_t* data, uint64_t size) :
        data_box(data, size)
    { }

    error box_error_code() const override
    {
        return error::invalid_mdat_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
