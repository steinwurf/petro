// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include <boost/iostreams/device/mapped_file.hpp>

namespace petro
{
namespace extractor
{
    class memory_mapped_file_layer
    {
    public:

        bool open(const std::string& filename)
        {
            assert(!m_file.is_open());
            m_file.open(filename);
            return m_file.is_open();
        }

        void close()
        {
            m_file.close();
        }

        const uint8_t* data() const
        {
            return (uint8_t*)m_file.data();
        }

        uint32_t data_size() const
        {
            return m_file.size();
        }

    private:

        boost::iostreams::mapped_file_source m_file;

    };
}
}
