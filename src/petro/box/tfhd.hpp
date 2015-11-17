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
    /// track fragment header
    class tfhd : public box
    {

    public:

        static const std::string TYPE;

    public:
        tfhd(byte_stream& bs, uint32_t size):
            box(tfhd::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string tfhd::TYPE = "tfhd";
}
}
