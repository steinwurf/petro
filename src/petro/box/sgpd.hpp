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
    /// sample group description
    class sgpd : public box
    {

    public:

        static const std::string TYPE;

    public:
        sgpd(byte_stream& bs, uint32_t size):
            box(sgpd::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string sgpd::TYPE = "sgpd";
}
}
