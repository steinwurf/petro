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
    /// chunk offset, partial data-offset information
    class stco : public box
    {

    public:

        static const std::string TYPE;

    public:
        stco(byte_stream& bs, uint32_t size):
            box(stco::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stco::TYPE = "stco";
}
}
