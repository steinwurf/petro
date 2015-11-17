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
    /// independent and disposable samples
    class sdtp : public box
    {

    public:

        static const std::string TYPE;

    public:
        sdtp(byte_stream& bs, uint32_t size):
            box(sdtp::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string sdtp::TYPE = "sdtp";
}
}
