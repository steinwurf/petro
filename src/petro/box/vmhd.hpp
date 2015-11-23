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
    /// video media header, overall information (video track only)
    class vmhd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(vmhd::TYPE, size, bs, parent);
            m_graphics_mode = bs.read_uint16_t();
            m_remaining_bytes -= 2;

            m_op_color.push_back(bs.read_uint16_t());
            m_remaining_bytes -= 2;
            m_op_color.push_back(bs.read_uint16_t());
            m_remaining_bytes -= 2;
            m_op_color.push_back(bs.read_uint16_t());
            m_remaining_bytes -= 2;

            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  graphics_mode: " << m_graphics_mode << std::endl;
            ss << "  op_color: #" <<
                (uint32_t)m_op_color[0] <<
                (uint32_t)m_op_color[1] <<
                (uint32_t)m_op_color[2] << std::endl;
            return ss.str();
        }

    private:
        /// specifies a composition mode for this video track, from the
        /// following enumerated set, which may be extended by derived
        /// specifications:
        /// copy = 0 copy over the existing image
        uint16_t m_graphics_mode;

        /// a set of 3 colour values (red, green, blue) available for use by
        /// graphics modes
        std::vector<uint16_t> m_op_color;
    };

    const std::string vmhd::TYPE = "vmhd";
}
}
