// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "annex_b_writer_layer.hpp"
#include "avc_sample_extractor.hpp"
#include "nalu_header_writer_layer.hpp"

namespace petro
{
namespace extractor
{
    /// Stack extracting nalu samples
    using annex_b_writer =
        annex_b_writer_layer<
        nalu_header_writer_layer<
        avc_sample_extractor>>;
}
}
