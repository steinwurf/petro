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
    /// item location
    class iloc : public box
    {

    public:

        static const std::string TYPE;

    public:
        iloc(byte_stream& bs, uint32_t size):
            box(iloc::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string iloc::TYPE = "iloc";
}
}
