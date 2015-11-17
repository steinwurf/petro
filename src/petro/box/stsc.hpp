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
    /// sample-to-chunk, partial data-offset information
    class stsc : public box
    {

    public:

        static const std::string TYPE;

    public:
        stsc(byte_stream& bs, uint32_t size):
            box(stsc::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stsc::TYPE = "stsc";
}
}
