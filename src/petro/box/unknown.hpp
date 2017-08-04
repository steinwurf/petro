// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <sstream>
#include <cassert>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// unknown box
class unknown : public box
{
public:

    unknown(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    virtual std::string type() const
    {
        return m_type + "?";
    }
};
}
}
