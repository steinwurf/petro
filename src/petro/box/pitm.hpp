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
    /// primary item reference
    class pitm : public box
    {

    public:

        static const std::string TYPE;

    public:
        pitm(byte_stream& bs, uint32_t size):
            box(pitm::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string pitm::TYPE = "pitm";
}
}
