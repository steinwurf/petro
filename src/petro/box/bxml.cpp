// Copyright Steinwurf ApS 2015.
// All Rights Reserved

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
