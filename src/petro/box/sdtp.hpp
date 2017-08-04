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
/// independent and disposable samples
class sdtp : public box
{

public:

    static const std::string TYPE;

public:
    sdtp(std::weak_ptr<box> parent) :
        box(sdtp::TYPE, parent)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        box::read(size, bs);
        bs.skip(m_remaining_bytes);
    }
};
}
}
