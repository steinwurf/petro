// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF STEINWURF
// The copyright notice above does not evidence any
// actual or intended publication of such source code.

#pragma once

#include "track_type.hpp"

#include <cassert>
#include <string>

namespace petro
{
namespace extractor
{
/// @return Human readable message corresponding to a track type
inline std::string track_type_to_string(petro::extractor::track_type type)
{
    switch (type)
    {
#define PETRO_TRACK_TYPE_TAG(id, msg)      \
    case petro::extractor::track_type::id: \
        return std::string(msg);
#include "track_type_tags.hpp"
#undef PETRO_TRACK_TYPE_TAG
    }
    assert(0 && "Invalid track type received!");
    return "";
}
}
}
