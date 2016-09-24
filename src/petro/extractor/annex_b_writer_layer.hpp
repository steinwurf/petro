// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include "avc_sample_access.hpp"

namespace petro
{
namespace extractor
{
/// Layer for writing an annex b formatted samples.
template<class Super>
class annex_b_writer_layer : public Super
{
public:

    //( Opens this and the underlying layers.)
    bool open()
    {
        if (!Super::open())
        {
            Super::close();
            return false;
        }
        update_state();
        return true;
    }

    //( Resets this and the underlying layers.
    void reset()
    {
        Super::reset();
        update_state();
    }

    //( Advances this and the underlying layers.
    void advance()
    {
        Super::advance();
        update_state();
    }

    /// Writes the annex b formatted sample. Exactly the size returned from
    /// annex_b_size will be used.
    void write_annex_b(uint8_t* buffer) const
    {
        assert(!Super::at_end());
        for (uint32_t i = 0; i < m_avc_sample_access.nalu_count(); ++i)
        {
            Super::write_nalu_header(buffer);
            buffer += Super::nalu_header_size();

            auto nalu = m_avc_sample_access.nalu_at(i);
            auto nalu_size = m_avc_sample_access.nalu_size_at(i);
            std::copy(nalu, nalu + nalu_size, buffer);
            buffer += nalu_size;
        }
    }

    /// Returns the number bytes that will be used in write_annex_b
    uint32_t annex_b_size() const
    {
        assert(!Super::at_end());
        uint32_t size = 0;
        for (uint32_t i = 0; i < m_avc_sample_access.nalu_count(); ++i)
        {
            size += Super::nalu_header_size();
            size += m_avc_sample_access.nalu_size_at(i);
        }
        return size;
    }

private:

    /// Updates the state of this layer.
    void update_state()
    {
        if (Super::at_end())
            return;

        m_avc_sample_access.read(
            Super::nalu_length_size(),
            Super::sample_data(),
            Super::sample_size());
    }

private:

    avc_sample_access m_avc_sample_access;
};
}
}
