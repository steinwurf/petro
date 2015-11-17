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
    /// binary XML container
    class bxml : public box
    {

    public:

        static const std::string TYPE;

    public:
        bxml(byte_stream& bs, uint32_t size):
            box(bxml::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string bxml::TYPE = "bxml";
}
}
