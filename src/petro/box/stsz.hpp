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
    /// sample sizes (framing)
    class stsz : public box
    {

    public:

        static const std::string TYPE;

    public:
        stsz(byte_stream& bs, uint32_t size):
            box(stsz::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stsz::TYPE = "stsz";
}
}
