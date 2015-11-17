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
    /// scheme information box
    class schi : public box
    {

    public:

        static const std::string TYPE;

    public:
        schi(byte_stream& bs, uint32_t size):
            box(schi::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string schi::TYPE = "schi";
}
}
