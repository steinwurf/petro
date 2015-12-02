// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <petro/box/all.hpp>

#include <gtest/gtest.h>

TEST(test_parser, read_data)
{
    auto test_filename = "test.mp4";
    std::ifstream test_mp4(test_filename, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());

    petro::parser<
        petro::box::mdat,
        petro::box::moov<petro::parser<petro::box::ftyp>>,
        petro::box::free,
        petro::box::ftyp> p;

    auto root = std::make_shared<petro::box::root>();
    petro::byte_stream bs(test_filename);
    p.read(root, bs);

    EXPECT_NE(nullptr, root->get_child("mdat"));
    EXPECT_NE(nullptr, root->get_child("moov"));
    EXPECT_NE(nullptr, root->get_child("ftyp"));
    EXPECT_NE(nullptr, root->get_child("free"));
    EXPECT_NE(nullptr, root->get_child("ftyp"));
}
