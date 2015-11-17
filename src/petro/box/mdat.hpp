// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include "../byte_stream.hpp"
#include "box.hpp"

#include <string>
#include <vector>

namespace petro
{
namespace box
{
    /// media data container
    class mdat : public box
    {

    public:

        static const std::string TYPE;

    public:
        mdat(byte_stream& bs, uint32_t size);

    private:

        const uint8_t* m_data;
        uint32_t m_size;
    };
}
}
