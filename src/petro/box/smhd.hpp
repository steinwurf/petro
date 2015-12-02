// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// sound media header, overall information (sound track only)
    class smhd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        smhd(std::weak_ptr<box> parent):
            full_box(smhd::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            full_box::read(size, bs);
            m_balance = bs.read_fixed_point_88();
            m_remaining_bytes -= 2;
            bs.skip(2);
            m_remaining_bytes -= 2;
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  balance: " << m_balance << std::endl;
            return ss.str();
        }

    private:

        /// a fixed-point 8.8 number that places mono audio tracks in a stereo
        /// space; 0 is center (the normal value); full left is -1.0 and full
        /// right is 1.0.
        float m_balance;
    };
}
}
