// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// binary XML container
class bxml : public box
{

public:

    static const std::string TYPE;

public:
    bxml(std::weak_ptr<box> parent);

    void read(uint32_t size, byte_stream& bs);
};
}
}
