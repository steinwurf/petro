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
    /// IPMP Information box
    class imif : public box
    {

    public:

        static const std::string TYPE;

    public:
        imif(byte_stream& bs, uint32_t size):
            box(imif::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string imif::TYPE = "imif";
}
}
