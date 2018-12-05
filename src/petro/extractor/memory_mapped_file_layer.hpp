// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <system_error>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem/operations.hpp>

namespace petro
{
namespace extractor
{
/// Provides the stack with data, and acts as the final layer.
class memory_mapped_file_layer
{
public:

    /// Opens the file. Remember to set the file path before calling this.
    /// Returns true if successful.
    void open(std::error_code& error)
    {
        assert(!m_file_path.empty());
        assert(!m_file.is_open());
        try
        {
            m_file.open(m_file_path);
        }
        catch (std::ios::failure& e)
        {
            error = std::make_error_code(std::errc::no_such_file_or_directory);
        }
    }

    /// Sets the file path of the fíle to open.
    void set_file_path(const std::string& file_path)
    {
        assert(!m_file.is_open());
        m_file_path = file_path;
    }

    /// Returns the set file path.
    std::string file_path() const
    {
        return m_file_path;
    }

    /// Closes the file.
    void close()
    {
        m_file.close();
    }

    /// Returns a pointer to the file
    const uint8_t* data() const
    {
        assert(m_file.is_open());
        return (uint8_t*)m_file.data();
    }

    /// Returns the size of the file
    uint64_t data_size() const
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
