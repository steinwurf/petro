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
    /// track fragment run
    class trun : public box
    {

    public:

        static const std::string TYPE;

    public:
        trun(byte_stream& bs, uint32_t size):
            box(trun::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string trun::TYPE = "trun";
}
}
