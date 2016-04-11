// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "bxml.hpp"

#include <string>

namespace petro
{
namespace box
{
    const std::string bxml::TYPE = "bxml";

    bxml::bxml(std::weak_ptr<box> parent):
        box(bxml::TYPE, parent)
    { }

    void bxml::read(uint64_t size, byte_stream& bs)
    {
        box::read(size, bs);
        bs.skip(m_remaining_bytes);
    }
}
}
