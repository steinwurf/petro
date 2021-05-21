// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../box/ctts.hpp"
#include "../box/mdhd.hpp"
#include "../box/mvhd.hpp"
#include "../box/stts.hpp"
#include "../decoding_time.hpp"
#include "../presentation_time.hpp"
#include <cassert>
#include <cstdint>
#include <string>

namespace petro
{
namespace extractor
{
/// Uses the aac and avc track layers to extract sample timestamps.
template <class Super>
class timestamp_extractor_layer : public Super
{
public:
    /// Open this an the underlying layers
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

        auto mdhd = trak->template get_child<box::mdhd>();
        if (mdhd == nullptr)
        {
            close();
            error = petro::error::mdhd_box_missing;
            return;
        }
        m_timescale = mdhd->timescale();

        auto stts = trak->template get_child<box::stts>();
        if (stts == nullptr)
        {
            close();
            error = petro::error::stts_box_missing;
            return;
        }

        m_stts = stts;
        m_ctts = trak->template get_child<box::ctts>();
    }

    /// Close this an the underlying layers
    void close()
    {
        m_timescale = 0;
        m_stts.reset();
        m_ctts.reset();

        Super::close();
    }

    /// Return the decoding timestamp related to the current sample
    uint64_t decoding_timestamp() const
    {
        assert(m_stts != nullptr);
        return decoding_time(m_stts, m_timescale, Super::sample_index());
    }

    /// Return the presentation timestamp related to the current sample
    uint64_t presentation_timestamp() const
    {
        assert(m_stts != nullptr);
        return presentation_time(m_stts, m_ctts, m_timescale,
                                 Super::sample_index());
    }

private:
    uint32_t m_timescale = 0;

    std::shared_ptr<const box::stts> m_stts;
    std::shared_ptr<const box::ctts> m_ctts;
};
}
}
