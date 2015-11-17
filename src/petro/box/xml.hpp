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
    /// XML container
    class xml : public box
    {

    public:

        static const std::string TYPE;

    public:
        xml(byte_stream& bs, uint32_t size):
            box(xml::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string xml::TYPE = "xml ";
}
}
