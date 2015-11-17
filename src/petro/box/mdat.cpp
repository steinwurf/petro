// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "mdat.hpp"
#include "../byte_stream.hpp"

#include <cassert>

namespace petro
{
namespace box
{
    const std::string mdat::TYPE = "mdat";

    mdat::mdat(byte_stream& bs, uint32_t size):
        box(mdat::TYPE),
        m_data(bs.data()),
        m_size(size)
    {
        bs.skip(m_size);
    }
}
}
