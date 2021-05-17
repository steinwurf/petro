// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "../box/avcc.hpp"
#include "../box/data_box.hpp"

namespace petro
{
namespace extractor
{
/// This layer exposes information about the avc track. This information is
/// used by the sample extractor layer to extract the avc samples.
template <class Super, class AvccBox = box::avcc>
class avc_track_layer : public Super
{
public:
    /// Open this and the underlying layer, returns false upon failure.
    void open(std::error_code& error)
    {
        assert(!error);
        Super::open(error);
        if (error)
        {
            Super::close();
            return;
        }

        auto trak = Super::trak();

        auto avc1 = trak->get_child("avc1");
        if (avc1 == nullptr)
        {
            Super::close();
            error = petro::error::avc1_box_missing;
            return;
        }

        m_avcc = avc1->template get_child<AvccBox>();
        if (m_avcc == nullptr)
        {
            Super::close();
            error = petro::error::avcc_box_missing;
            return;
        }
    }

    /// Return a pointer to the pps data
    const uint8_t* pps_data() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->picture_parameter_set(0)->data();
    }

    /// Return the size of the pps data
    uint64_t pps_size() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->picture_parameter_set(0)->size();
    }

    /// Return a pointer to the sps data
    const uint8_t* sps_data() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->sequence_parameter_set(0)->data();
    }

    /// Return the size of the sps data
    uint64_t sps_size() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->sequence_parameter_set(0)->size();
    }

    /// Return the size of the length preceeded each nalu sample in the h264
    /// sample.
    uint8_t nalu_length_size() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->length_size();
    }

    /// Close this and the underlying layer.
    void close()
    {
        m_avcc.reset();
        Super::close();
    }

private:
    std::shared_ptr<const AvccBox> m_avcc;
};
}
}
