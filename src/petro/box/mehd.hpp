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
    /// movie extends header box
    class mehd : public box
    {

    public:

        static const std::string TYPE;

    public:
        mehd(byte_stream& bs, uint32_t size):
            box(mehd::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string mehd::TYPE = "mehd";
}
}
