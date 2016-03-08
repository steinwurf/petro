// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <sstream>
#include <vector>

namespace petro
{
    class picture_parameter_set
    {
    public:
        picture_parameter_set(std::vector<uint8_t> data):
            m_data(data)
        { }

        const uint8_t* data() const
        {
            return m_data.data();
        }

        uint32_t size() const
        {
            return m_data.size();
        }

        std::string describe() const
        {
            std::stringstream ss;
            ss << "pps:" << std::endl;
            ss << "      size: " << size() << std::endl;

            return ss.str();
        }

    private:

        std::vector<uint8_t> m_data;
    };
}
