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
/// Uses the mvhd box to extract the media duration.
template <class Super>
class media_duration_layer : public Super
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

        auto root = Super::root();
        auto mvhd = root->template get_child<box::mvhd>();
        if (mvhd == nullptr)
        {
            close();
            error = petro::error::mvhd_box_missing;
            return;
        }
        m_media_duration = mvhd->duration() * 1000000 / mvhd->timescale();
    }

    /// Close this an the underlying layers
    void close()
    {
        m_media_duration = 0;
        Super::close();
    }

    /// Return the total media duration in microseconds
    uint64_t media_duration() const
    {
        return m_media_duration;
    }

private:
    // Total media duration in microseconds
    uint64_t m_media_duration = 0;
};
}
}
