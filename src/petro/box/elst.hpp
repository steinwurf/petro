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
    /// an edit list
    class elst : public box
    {

    public:

        static const std::string TYPE;

    public:
        elst(byte_stream& bs, uint32_t size):
            box(elst::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string elst::TYPE = "elst";
}
}
