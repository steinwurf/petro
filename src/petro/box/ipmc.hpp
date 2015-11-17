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
    /// IPMP Control Box
    class ipmc : public box
    {

    public:

        static const std::string TYPE;

    public:
        ipmc(byte_stream& bs, uint32_t size):
            box(ipmc::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string ipmc::TYPE = "ipmc";
}
}
