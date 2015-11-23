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
    /// root object
    class root : public box
    {
    public:

        /// hide box implementation
        void read(const std::string& type, uint32_t size, byte_stream& bs, box* parent) override
        {
            (void) type;
            (void) size;
            (void) bs;
            (void) parent;
            // root cannot be read.
            assert(0);
        }
    };
}
}
