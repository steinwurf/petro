// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "box.hpp"

namespace petro
{
namespace box
{
/// root object
class root : public box
{
public:

    std::string type() const override
    {
        return "root";
    }

    std::string describe() const override
    {
        std::stringstream ss;
        ss << type() << std::endl;
        return ss.str();
    }
};
}
}
