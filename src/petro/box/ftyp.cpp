// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "ftyp.hpp"
#include "../byte_stream.hpp"

#include <cassert>
#include <sstream>

namespace petro
{
namespace box
{
    const std::string ftyp::TYPE = "ftyp";

    ftyp::ftyp(uint32_t size, byte_stream& bs, box* parent):
        box(ftyp::TYPE, size, bs, parent)
    {
        m_major_brand = bs.read_type();
        m_remaining_bytes -= 4;

        m_minor_version = bs.read_uint32_t();
        m_remaining_bytes -= 4;

        assert(m_remaining_bytes % 4 == 0);

        while(m_remaining_bytes != 0)
        {
            m_compatible_brands.push_back(bs.read_type());
            m_remaining_bytes -= 4;
        }
    }


    std::string ftyp::describe() const
    {
        std::stringstream ss;
        ss << box::describe() << std::endl;
        ss << "  major_brand: " << m_major_brand << std::endl;
        ss << "  minor_version: " << m_minor_version << std::endl;

        ss << "  compatible_brands: ";
        auto seperator = "";
        for (const auto& brand : m_compatible_brands)
        {
            ss << seperator << brand;
            seperator =  ", ";
        }
        ss << std::endl;
        return ss.str();
    }
}
}
