// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

namespace petro
{
namespace box
{
/// Null media header, overall information (some tracks only)
class nmhd : public full_box
{

public:
    static const std::string TYPE;

public:
    nmhd(const uint8_t* data, uint64_t size) : full_box(data, size)
    {
    }

    error box_error_code() const override
    {
        return error::invalid_nmhd_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};
}
}
