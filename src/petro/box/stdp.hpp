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
    /// sample degradation priority
    class stdp : public box
    {

    public:

        static const std::string TYPE;

    public:
        stdp(byte_stream& bs, uint32_t size):
            box(stdp::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string stdp::TYPE = "stdp";
}
}
