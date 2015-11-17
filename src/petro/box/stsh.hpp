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
    /// shadow sync sample table
    class stsh : public box
    {

    public:

        static const std::string TYPE;

    public:
        stsh(byte_stream& bs, uint32_t size):
            box(stsh::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stsh::TYPE = "stsh";
}
}
