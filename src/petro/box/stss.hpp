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
    /// sync sample table (random access points)
    class stss : public box
    {

    public:

        static const std::string TYPE;

    public:
        stss(byte_stream& bs, uint32_t size):
            box(stss::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stss::TYPE = "stss";
}
}
