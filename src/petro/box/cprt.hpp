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
    /// copyright etc.
    class cprt : public box
    {

    public:

        static const std::string TYPE;

    public:
        cprt(byte_stream& bs, uint32_t size):
            box(cprt::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string cprt::TYPE = "cprt";
}
}
