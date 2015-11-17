// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// item information
    class iinf : public box
    {

    public:

        static const std::string TYPE;

    public:
        iinf(byte_stream& bs, uint32_t size):
            box(iinf::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string iinf::TYPE = "iinf";
}
}
