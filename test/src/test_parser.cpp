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
    std::ifstream test_mp4("test.mp4", std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());
    std::vector<char> data((std::istreambuf_iterator<char>(test_mp4)),
                            std::istreambuf_iterator<char>());
    petro::parser<
        petro::box::mdat,
        petro::box::moov<petro::parser<petro::box::ftyp>>,
        petro::box::free,
        petro::box::ftyp> p;

    std::vector<petro::box::box*> boxes;

    auto root = std::make_shared<petro::box::root>();
    petro::byte_stream bs((uint8_t*)data.data(), data.size());
    p.read(root, bs);

    for(const auto& box : root->children())
    {
        std::cout << box->type() << std::endl;
    }
}
