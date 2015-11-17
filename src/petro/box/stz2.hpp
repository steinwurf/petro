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
    /// compact sample sizes (framing)
    class stz2 : public box
    {

    public:

        static const std::string TYPE;

    public:
        stz2(byte_stream& bs, uint32_t size):
            box(stz2::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stz2::TYPE = "stz2";
}
}
