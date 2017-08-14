// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>
#include <cstdint>
#include "box/stts.hpp"
#include "box/ctts.hpp"

namespace petro
{
/// returns the sample's presentation time in microseconds.
/// the ctts box is allowed to be zero.
inline int64_t presentation_time(
    std::shared_ptr<const petro::box::stts> stts,
    std::shared_ptr<const petro::box::ctts> ctts,
    uint32_t media_header_timescale,
    uint32_t sample_index)
{
    assert(stts != nullptr);
    assert(media_header_timescale != 0);
    assert(sample_index < stts->samples());

    int64_t presentation_time = stts->decoding_time(sample_index);

    if (ctts != nullptr)
    {
        assert(sample_index < ctts->samples());
        presentation_time += ctts->composition_time(sample_index);
    }

    presentation_time *= 1000000;

    presentation_time /= media_header_timescale;

    return presentation_time;
}
}
