// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "base_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// root object
class root : public base_box
{
public:

    std::string type() const
    {
        return "root";
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << type() << std::endl;
        return ss.str();
    }
};
}
}
