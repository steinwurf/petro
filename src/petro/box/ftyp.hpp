// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../byte_stream.hpp"
#include "../helper.hpp"
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

    ftyp(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    void parse_box_content(std::error_code& error) override
    {
        uint32_t major_brand_value;
        m_bs.read(major_brand_value, error);
        if (error)
            return;
        m_major_brand = helper::type(major_brand_value);
        m_bs.read(m_minor_version);
        if (error)
            return;

        while (m_bs.remaining_size() != 0)
        {
            uint32_t compatible_brand_value;
            m_bs.read(compatible_brand_value, error);
            if (error)
                return;

            m_compatible_brands.push_back(helper::type(compatible_brand_value));
        }
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
