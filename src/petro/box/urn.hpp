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
    /// urn
    class urn : public box
    {

    public:

        static const std::string TYPE;

    public:
        urn(byte_stream& bs, uint32_t size):
            box(urn::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string urn::TYPE = "urn ";
}
}
