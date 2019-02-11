// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/track_extractor.hpp>
#include <petro/extractor/track_type_to_string.hpp>
#include <petro/extractor/file.hpp>

#include <cstdint>
#include <vector>
#include <fstream>

#include <gtest/gtest.h>

TEST(extractor_test_track_extractor, init)
{
    std::error_code error;
    petro::extractor::file file;
    file.open("test1.mp4", error);
    ASSERT_FALSE(bool(error));

    petro::extractor::track_extractor extractor;

    extractor.open(file.data(), file.size(), error);
    SCOPED_TRACE(testing::Message() << error.message());
    ASSERT_FALSE(bool(error));

    auto tracks = extractor.tracks();
    std::vector<petro::extractor::track_extractor::track> expected_tracks = {
        { 1, petro::extractor::track_type::avc1 },
        { 2, petro::extractor::track_type::aac }
    };

    std::stringstream ss;
    for (auto track : tracks)
    {
        ss << "- Track " << track.id << ": "
            << petro::extractor::track_type_to_string(track.type) << '\n';
    }
    SCOPED_TRACE(testing::Message() << "Available tracks:\n" << ss.str());
    ASSERT_EQ(expected_tracks.size(), tracks.size());
    EXPECT_EQ(expected_tracks.at(0).id, tracks.at(0).id);
    EXPECT_EQ(expected_tracks.at(0).type, tracks.at(0).type);
    EXPECT_EQ(expected_tracks.at(1).id, tracks.at(1).id);
    EXPECT_EQ(expected_tracks.at(1).type, tracks.at(1).type);
}
