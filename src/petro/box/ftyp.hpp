// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_box.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace petro
{
namespace box
{
/// file type and compatibility
class ftyp : public data_box
{

public:
    static const std::string TYPE;

public:
    ftyp(const uint8_t* data, uint64_t size) : data_box(data, size)
    {
    }

    void parse_box_content(std::error_code& error) override
    {
        m_bs.read(m_major_brand, 4, error);
        if (error)
            return;
        m_bs.read<uint32_t>(m_minor_version, error);
        if (error)
            return;

        while (m_bs.remaining_size() != 0)
        {
            std::string compatible_brand;
            m_bs.read(compatible_brand, 4, error);
            if (error)
                return;

            m_compatible_brands.push_back(compatible_brand);
        }
    }

    error box_error_code() const override
    {
        return error::invalid_ftyp_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string box_describe() const override
    {
        std::stringstream ss;
        ss << "  major_brand: " << m_major_brand << std::endl;
        ss << "  minor_version: " << m_minor_version << std::endl;

        ss << "  compatible_brands: ";
        auto seperator = "";
        for (const auto& brand : m_compatible_brands)
        {
            ss << seperator << brand;
            seperator = ", ";
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
