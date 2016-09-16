// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

#include "read_bytes.hpp"

namespace petro
{
namespace extractor
{
    /// Class for accesing the elements in an avc sample.
    class avc_sample_access
    {

    public:

        void read(uint32_t size_length, const uint8_t* data, uint32_t size)
        {
            m_nalus.clear();
            m_nalu_sizes.clear();

            while(size != 0)
            {
                auto nalu_size = detail::read_bytes(size_length, data);
                data += size_length;
                size -= size_length;
                m_nalus.push_back(data);
                data += nalu_size;
                size -= nalu_size;
                m_nalu_sizes.push_back(nalu_size);
            }
        }

        uint32_t nalu_count() const
        {
            return m_nalus.size();
        }

        const uint8_t* nalu_at(uint32_t index) const
        {
            assert(index < m_nalus.size());
            return m_nalus[index];
        }

        uint32_t nalu_size_at(uint32_t index) const
        {
            assert(index < m_nalu_sizes.size());
            return m_nalu_sizes[index];
        }

    private:

        std::vector<const uint8_t*> m_nalus;
        std::vector<uint32_t> m_nalu_sizes;
    };
}
}
