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
    /// track reference container
    class tref : public box
    {

    public:

        static const std::string TYPE;

    public:
        tref(byte_stream& bs, uint32_t size):
            box(tref::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string tref::TYPE = "tref";
}
}
