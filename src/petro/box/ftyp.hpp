// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../byte_stream.hpp"
#include "box.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace petro
{
namespace box
{
/// file type and compatibility
class ftyp : public box
{

public:

    static const std::string TYPE;

public:
    ftyp(std::weak_ptr<box> parent) :
        box(ftyp::TYPE, parent)
    { }

    void read(uint64_t size, byte_stream& bs)
    {
        box::read(size, bs);
        m_major_brand = bs.read_type();
        m_remaining_bytes -= 4;
        m_minor_version = bs.read_uint32_t();
        m_remaining_bytes -= 4;

        assert(m_remaining_bytes % 4 == 0);

        while (m_remaining_bytes != 0)
        {
            m_compatible_brands.push_back(bs.read_type());
            m_remaining_bytes -= 4;
        }
        assert(m_remaining_bytes == 0);
    }
    virtual std::string describe() const
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

private:

    /// a brand identifier
    std::string m_major_brand;

    /// an informative integer for the minor version of the major brand
    uint32_t m_minor_version;

    /// a list, to the end of the box, of brands
    std::vector<std::string> m_compatible_brands;
};

}
}
