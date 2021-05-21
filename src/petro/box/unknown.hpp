// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

#include "data_box.hpp"

namespace petro
{
namespace box
{
/// unknown box
class unknown : public data_box
{
public:
    unknown(const uint8_t* data, uint64_t size) : data_box(data, size)
    {
    }

    std::string type() const override
    {
        return m_type;
    }

    std::string box_describe() const override
    {
        std::stringstream ss;
        ss << "  unknown" << std::endl;
        return ss.str();
    }
};
}
}
