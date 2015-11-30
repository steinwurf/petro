// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cassert>
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

        root():
            box("root", std::weak_ptr<box>())
        { }

        /// hide box implementation
        void read(uint32_t size, byte_stream& bs) override
        {
            (void) size;
            (void) bs;
            // root cannot be read.
            assert(0);
        }
    };
}
}
