// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <system_error>

namespace petro
{
namespace extractor
{
/// Provides the stack with data, and acts as the final layer.
class data_layer
{
public:

    /// Opens the extractor. Remember to set the data buffer before calling
    /// this.
    void open(std::error_code& error)
    {
        (void) error;
        assert(m_data != nullptr);
        assert(m_size > 0);
    }

    /// Sets the data buffer
    void set_buffer(const uint8_t* data, uint64_t size)
    {
        m_data = data;
        m_size = size;
    }

    void close()
    {
        m_data = nullptr;
        m_size = 0;
    }

    /// Returns a pointer to the file
    const uint8_t* data() const
    {
        assert(m_data != nullptr);
        return (uint8_t*)m_data;
    }

    /// Returns the size of the file
    uint64_t data_size() const
    {
        assert(m_size > 0);
        return m_size;
    }

private:
    const uint8_t* m_data = nullptr;
    uint64_t m_size = 0;
};
}
}
