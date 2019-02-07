// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor2/file.hpp>
#include <petro/extractor2/track_extractor.hpp>
#include <petro/extractor2/sample_extractor.hpp>
#include <petro/extractor2/aac_sample_extractor.hpp>
#include <petro/extractor2/h264_sample_extractor.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

TEST(extractor2_test_mp4_file, init)
{

}

TEST(extractor2_test_mp4_file, track_extractor)
{
    std::error_code error;
    petro::extractor::file f;
    f.open("test1.mp4", error);
    ASSERT_FALSE(bool(error));

    petro::extractor::track_extractor extractor;
    extractor.open(f.data(), f.size(), error);
    ASSERT_FALSE(bool(error));

    auto tracks = extractor.tracks();
    for (auto track : tracks)
    {
        switch(track.type)
        {
    //         case petro::extractor::track_type::aac:
    //         {
    //             petro::extractor::aac_sample_extractor aac_sample_extractor;
    //             aac_sample_extractor.open(f.data(), f.size(), track_id, error);
    //             break;
    //         }
    //         case petro::extractor::track_type::h264:
    //         {
    //             petro::extractor::h264_sample_extractor h264_sample_extractor;
    //             h264_sample_extractor.open(f.data(), f.size(), track_id, error);
    //             break;
    //         }
    //         case petro::extractor::track_type::unknown:
    //         {
    //             petro::extractor::sample_extractor sample_extractor;
    //             sample_extractor.open(f.data(), f.size(), track_id, error);
    //             break;
    //         }
    //         default:
    //             break;
        }
    }
    extractor.close();
}
