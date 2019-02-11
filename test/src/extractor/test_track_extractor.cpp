// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/track_extractor.hpp>
#include <petro/extractor/file.hpp>

#include <cstdint>
#include <vector>
#include <fstream>

#include <gtest/gtest.h>

TEST(extractor_test_track_extractor, init)
{
    petro::extractor::track_extractor extractor;
}
