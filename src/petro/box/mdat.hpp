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
        mdat(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(mdat::TYPE, size, bs, parent),
            m_data(bs.data())
        {
            bs.skip(m_remaining_bytes);
        }

        const uint8_t* data() const
        {
            return m_data;
        }

    private:

        const uint8_t* m_data;
    };

    const std::string mdat::TYPE = "mdat";
}
}
