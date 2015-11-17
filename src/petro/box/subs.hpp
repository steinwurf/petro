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
    /// sub-sample information
    class subs : public box
    {

    public:

        static const std::string TYPE;

    public:
        subs(byte_stream& bs, uint32_t size):
            box(subs::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string subs::TYPE = "subs";
}
}
