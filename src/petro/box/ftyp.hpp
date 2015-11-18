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
    /// file type and compatibility
    class ftyp : public box
    {

    public:

        static const std::string TYPE;

    public:
        ftyp(uint32_t size, byte_stream& bs, box* parent=nullptr);
        virtual std::string describe() const;

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
