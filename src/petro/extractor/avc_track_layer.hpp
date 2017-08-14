// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <memory>

#include "../box/data_box.hpp"
#include "../box/avcc.hpp"

namespace petro
{
namespace extractor
{
/// This layer exposes information about the avc track. This information is
/// used by the sample extractor layer to extract the avc samples.
template<class Super>
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

        auto root = Super::root();

        auto avc1 = root->get_child("avc1");
        if (avc1 == nullptr)
        {
            Super::close();
            error = petro::error::avc1_box_missing;
            return;
        }

        auto avcc = avc1->template get_child<box::avcc>();
        if (avcc == nullptr)
        {
            Super::close();
            error = petro::error::avcc_box_missing;
            return;
        }

        auto trak = avc1->get_parent("trak");
        if (trak == nullptr)
        {
            Super::close();
            error = petro::error::trak_box_missing;
            return;
        }

        m_pps_data = avcc->picture_parameter_set(0)->data();
        m_pps_size = avcc->picture_parameter_set(0)->size();
        m_sps_data = avcc->sequence_parameter_set(0)->data();
        m_sps_size = avcc->sequence_parameter_set(0)->size();
        m_nalu_length_size = avcc->length_size();

        m_trak = trak;
    }

    /// Return a shared pointer to the h264 trak
    std::shared_ptr<const box::box> trak() const
    {
        assert(m_trak != nullptr);
        return m_trak;
    }

    /// Return a pointer to the pps data
    const uint8_t* pps_data() const
    {
        return m_pps_data;
    }

    /// Return the size of the pps data
    uint32_t pps_size() const
    {
        return m_pps_size;
    }

    /// Return a pointer to the sps data
    const uint8_t* sps_data() const
    {
        return m_sps_data;
    }

    /// Return the size of the sps data
    uint32_t sps_size() const
    {
        return m_sps_size;
    }

    /// Return the size of the length preceeded each nalu sample in the h264
    /// sample.
    uint8_t nalu_length_size() const
    {
        return m_nalu_length_size;
    }

    /// Close this and the underlying layer.
    void close()
    {
        m_trak.reset();
        m_pps_data = nullptr;
        m_pps_size = 0;
        m_sps_data = nullptr;
        m_sps_size = 0;
        m_nalu_length_size = 0;
        Super::close();
    }

private:

    std::shared_ptr<const box::box> m_trak;

    const uint8_t* m_pps_data;
    uint32_t m_pps_size;
    const uint8_t* m_sps_data;
    uint32_t m_sps_size;
    uint8_t m_nalu_length_size;
};
}
}
