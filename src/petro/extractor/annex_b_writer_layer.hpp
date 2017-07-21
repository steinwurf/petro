// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <algorithm>

namespace petro
{
namespace extractor
{
/// Layer for writing an annex b formatted samples.
template<class Super>
class annex_b_writer_layer : public Super
{
public:

    /// Writes the annex b formatted sample. Exactly the size returned from
    /// annex_b_size will be used.
    void write_annex_b(uint8_t* buffer) const
    {
        for (uint32_t i = 0; i < Super::nalu_count(); ++i)
        {
            Super::write_nalu_header(buffer);
            buffer += Super::nalu_header_size();

            auto nalu = Super::nalu_at(i);
            auto nalu_size = Super::nalu_size_at(i);
            std::copy(nalu, nalu + nalu_size, buffer);
            buffer += nalu_size;
        }
    }

    /// Returns the number bytes that will be used in write_annex_b
    uint32_t annex_b_size() const
    {
        uint32_t size = 0;
        for (uint32_t i = 0; i < Super::nalu_count(); ++i)
        {
            size += Super::nalu_header_size();
            size += Super::nalu_size_at(i);
        }
        return size;
    }
};
}
}
