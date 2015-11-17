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
    /// scheme type box
    class schm : public box
    {

    public:

        static const std::string TYPE;

    public:
        schm(byte_stream& bs, uint32_t size):
            box(schm::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string schm::TYPE = "schm";
}
}
