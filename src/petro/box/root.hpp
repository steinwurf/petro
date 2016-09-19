// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

            root() :
                box("root", std::weak_ptr<box>())
            { }

            /// hide box implementation
            void read(uint64_t size, byte_stream& bs) override
            {
                (void) size;
                (void) bs;
                // root cannot be read.
                assert(0);
            }
        };
    }
}
