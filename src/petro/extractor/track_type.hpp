// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

namespace petro
{
namespace extractor
{
/// Enumeration of different nalu types, we use a bit of macro
/// uglyness to makes this easy. PHK says this is ok so if you have a
/// problem with it - take it up with him :)
///
/// http://phk.freebsd.dk/time/20141116.html
enum class track_type
{
#define PETRO_TRACK_TYPE_TAG(id,msg) id,
#include "track_type_tags.hpp"
#undef PETRO_TRACK_TYPE_TAG
};
}
}
