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
    /// sample padding bits
    class padb : public box
    {

    public:

        static const std::string TYPE;

    public:
        padb(byte_stream& bs, uint32_t size):
            box(padb::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string padb::TYPE = "padb";
}
}
