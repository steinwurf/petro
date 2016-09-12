// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace petro
{
namespace extractor
{
    template<class Super>
    class nalu_header_writer_layer : public Super
    {
    public:

        void write_nalu_header(uint8_t* data) const
        {
            data[0] = 0x00;
            data[1] = 0x00;
            data[2] = 0x00;
            data[3] = 0x01;
        }

        uint32_t nalu_header_size() const
        {
            return 4U;
        }
    };
}
}
