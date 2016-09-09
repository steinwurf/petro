// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "timestamp_extractor_layer.hpp"
#include "sample_extractor_layer.hpp"
#include "h264_track_layer.hpp"
#include "parser_layer.hpp"
#include "memory_mapped_file_layer.hpp"

namespace petro
{
namespace extractor
{
    using h264_sample_extractor =
        timestamp_extractor_layer<
        sample_extractor_layer<
        h264_track_layer<
        parser_layer<
        memory_mapped_file_layer>>>>;
}
}
