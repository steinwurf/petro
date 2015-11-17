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
    /// 64-bit chunk offset
    class co64 : public box
    {

    public:

        static const std::string TYPE;

    public:
        co64(byte_stream& bs, uint32_t size):
            box(co64::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string co64::TYPE = "co64";
}
}
