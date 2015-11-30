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
        mdat(std::weak_ptr<box> parent):
            box(mdat::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            m_data_offset = bs.size() - bs.remaining_bytes();
            bs.skip(m_remaining_bytes);
        }

        uint32_t data_offset() const
        {
            return m_data_offset;
        }

        std::string describe() const
        {
            std::stringstream ss;
            ss << box::describe() << std::endl;
            ss << "  data_offset: " << m_data_offset << std::endl;
            return ss.str();
        }

    private:

        uint32_t m_data_offset;
    };
}
}
