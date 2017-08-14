// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <system_error>

#include "read_bytes.hpp"

namespace petro
{
namespace extractor
{

/// Layer for accesing the nalus elements in an avc sample.
template<class Super>
class avc_sample_access_layer : public Super
{
public:

    /// Opens this and the underlying layers.
    void open(std::error_code& error)
    {
        assert(!error);
        Super::open(error);
        if (error)
        {
            Super::close();
            return;
        }
        update_state();
    }

    /// Resets this and the underlying layers.
    void reset()
    {
        Super::reset();
        update_state();
    }

    /// Advances this and the underlying layers.
    void advance()
    {
        Super::advance();
        update_state();
    }

    uint32_t nalu_count() const
    {
        assert(!Super::at_end());
        return m_nalus.size();
    }

    const uint8_t* nalu_at(uint32_t index) const
    {
        assert(!Super::at_end());
        assert(index < m_nalus.size());
        return m_nalus[index];
    }

    uint32_t nalu_size_at(uint32_t index) const
    {
        assert(!Super::at_end());
        assert(index < m_nalu_sizes.size());
        return m_nalu_sizes[index];
    }

private:

    /// Updates the state of this layer.
    void update_state()
    {
        m_nalus.clear();
        m_nalu_sizes.clear();

        if (Super::at_end())
            return;

        const uint8_t* data = Super::sample_data();
        uint32_t size = Super::sample_size();

        while (size != 0)
        {
            auto nalu_size = detail::read_bytes(Super::nalu_length_size(), data);
            data += Super::nalu_length_size();
            size -= Super::nalu_length_size();
            m_nalus.push_back(data);
            data += nalu_size;
            size -= nalu_size;
            m_nalu_sizes.push_back(nalu_size);
        }
    }

private:

    std::vector<const uint8_t*> m_nalus;
    std::vector<uint32_t> m_nalu_sizes;
};
}
}
