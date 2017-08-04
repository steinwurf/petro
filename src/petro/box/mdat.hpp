// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../byte_stream.hpp"
#include "box.hpp"

#include <string>
#include <vector>

namespace petro
{
namespace box
{
/// media data container
class mdat : public box
{

public:

    static const std::string TYPE;

public:
    mdat(std::weak_ptr<box> parent) :
        box(mdat::TYPE, parent)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        box::read(size, bs);
        bs.skip(m_remaining_bytes);
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << box::describe() << std::endl;
        return ss.str();
    }
};
}
}
