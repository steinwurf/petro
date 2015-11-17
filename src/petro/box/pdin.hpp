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
    /// progressive download information
    class pdin : public box
    {

    public:

        static const std::string TYPE;

    public:
        pdin(byte_stream& bs, uint32_t size):
            box(pdin::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string pdin::TYPE = "pdin";
}
}
