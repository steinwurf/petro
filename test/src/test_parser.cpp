// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/parser.hpp>

#include <cstdint>
#include <iterator>
#include <memory>
#include <sstream>
#include <vector>

#include <petro/box/free.hpp>
#include <petro/box/ftyp.hpp>
#include <petro/box/mdat.hpp>
#include <petro/box/moov.hpp>
#include <petro/box/mvhd.hpp>
#include <petro/box/root.hpp>
#include <petro/box/trak.hpp>
#include <petro/box/udta.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

#include <gtest/gtest.h>

TEST(test_parser, read_data)
{
    auto test_filename = "test1.mp4";
    boost::iostreams::mapped_file_source test_mp4(test_filename);

    ASSERT_TRUE(test_mp4.is_open());

    using trak_type = petro::box::trak<petro::parser<>>;

    using moov_type =
        petro::box::moov<petro::parser<petro::box::mvhd, trak_type>>;

    petro::parser<petro::box::mdat, moov_type, petro::box::free,
                  petro::box::ftyp>
        p;

    auto root = std::make_shared<petro::box::root>();

    std::error_code error;
    p.parse((uint8_t*)test_mp4.data(), test_mp4.size(), root, error);
    ASSERT_FALSE(bool(error));

    // check that we can get the children and cast them to their appropiate
    // type.
    auto mdat_box = root->get_child("mdat");
    EXPECT_NE(nullptr, mdat_box);
    auto mdat = std::dynamic_pointer_cast<const petro::box::mdat>(mdat_box);
    EXPECT_NE(nullptr, mdat);

    auto ftyp_box = root->get_child("ftyp");
    EXPECT_NE(nullptr, ftyp_box);
    auto ftyp = std::dynamic_pointer_cast<const petro::box::ftyp>(ftyp_box);
    EXPECT_NE(nullptr, ftyp);

    auto free_box = root->get_child("free");
    EXPECT_NE(nullptr, free_box);
    auto free = std::dynamic_pointer_cast<const petro::box::free>(free_box);
    EXPECT_NE(nullptr, free);

    auto moov_box = root->get_child("moov");
    EXPECT_NE(nullptr, moov_box);
    auto moov = std::dynamic_pointer_cast<const moov_type>(moov_box);
    EXPECT_NE(nullptr, moov);

    auto mvhd_box = root->get_child("mvhd");
    EXPECT_NE(nullptr, mvhd_box);
    auto mvhd = std::dynamic_pointer_cast<const petro::box::mvhd>(mvhd_box);
    EXPECT_NE(nullptr, mvhd);

    auto trak_box = root->get_child("trak");
    EXPECT_NE(nullptr, trak_box);
    auto trak = std::dynamic_pointer_cast<const trak_type>(trak_box);
    EXPECT_NE(nullptr, trak);
}

TEST(test_parser, no_boxes)
{
    auto test_filename = "test1.mp4";
    boost::iostreams::mapped_file_source test_mp4(test_filename);
    ASSERT_TRUE(test_mp4.is_open());

    petro::parser<> p;

    auto root = std::make_shared<petro::box::root>();

    std::error_code error;
    p.parse((uint8_t*)test_mp4.data(), test_mp4.size(), root, error);
    ASSERT_FALSE(bool(error));

    // check that the children can be found, but only as basic boxes.
    auto mdat_box = root->get_child("mdat");
    EXPECT_NE(nullptr, mdat_box);
    auto mdat = std::dynamic_pointer_cast<const petro::box::mdat>(mdat_box);
    EXPECT_EQ(nullptr, mdat);

    auto ftyp_box = root->get_child("ftyp");
    EXPECT_NE(nullptr, ftyp_box);
    auto ftyp = std::dynamic_pointer_cast<const petro::box::ftyp>(ftyp_box);
    EXPECT_EQ(nullptr, ftyp);

    auto free_box = root->get_child("free");
    EXPECT_NE(nullptr, free_box);
    auto free = std::dynamic_pointer_cast<const petro::box::free>(free_box);
    EXPECT_EQ(nullptr, free);
}
