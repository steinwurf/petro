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
    /// movie fragment random access offset
    class mfro : public box
    {

    public:

        static const std::string TYPE;

    public:
        mfro(byte_stream& bs, uint32_t size):
            box(mfro::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string mfro::TYPE = "mfro";
}
}
