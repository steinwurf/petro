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
    /// movie fragment header
    class mfhd : public box
    {

    public:

        static const std::string TYPE;

    public:
        mfhd(byte_stream& bs, uint32_t size):
            box(mfhd::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string mfhd::TYPE = "mfhd";
}
}
