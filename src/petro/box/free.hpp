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
    /// free space
    class free : public box
    {

    public:

        static const std::string TYPE;

    public:
        free(byte_stream& bs, uint32_t size):
            box(free::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string free::TYPE = "free";
}
}
