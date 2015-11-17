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
    /// track fragment random access
    class tfra : public box
    {

    public:

        static const std::string TYPE;

    public:
        tfra(byte_stream& bs, uint32_t size):
            box(tfra::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string tfra::TYPE = "tfra";
}
}
