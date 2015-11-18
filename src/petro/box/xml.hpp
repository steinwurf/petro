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
        xml(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(xml::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string xml::TYPE = "xml ";
}
}
