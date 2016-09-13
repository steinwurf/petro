// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "h264_sample_extractor.hpp"
#include "h264_nalu_extractor_layer.hpp"
#include "nalu_header_writer_layer.hpp"

namespace petro
{
namespace extractor
{
    /// Stack extracting nalu samples useful for creating annex b formatted
    /// sh264.
    using h264_nalu_extractor =
        nalu_header_writer_layer<
        h264_nalu_extractor_layer<
        h264_sample_extractor>>;
}
}
