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

        bool open()
        {
            assert(!m_file_path.empty());
            assert(!m_file.is_open());
            m_file.open(m_file_path);
            return m_file.is_open();
        }

        void set_file_path(const std::string& file_path)
        {
            assert(!m_file.is_open());
            m_file_path = file_path;
        }

        std::string file_path() const
        {
            assert(!m_file.is_open());
            return m_file_path;
        }

        void close()
        {
            m_file.close();
        }

        const uint8_t* data() const
        {
            assert(m_file.is_open());
            return (uint8_t*)m_file.data();
        }

        uint32_t data_size() const
        {
            assert(m_file.is_open());
            return m_file.size();
        }

    private:

        boost::iostreams::mapped_file_source m_file;
        std::string m_file_path;
    };
}
}
