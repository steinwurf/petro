// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

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
    vmhd(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read<endian::u16>(m_graphics_mode, error);
        if (error)
            return;
        std::vector<uint16_t> op_color(3);
        for (uint32_t i = 0; i < op_color.size(); ++i)
        {
            uint16_t value = 0;
            m_bs.read<endian::u16>(value, error);
            if (error)
                return;
            op_color[i] = value;
        }
        m_op_color = op_color;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_vmhd_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        assert(m_op_color.size() != 0);
        std::stringstream ss;
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
}
}
