// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/annex_b_writer_layer.hpp>

#include <stub/function.hpp>
#include <gtest/gtest.h>

namespace
{
struct dummy_layer
{
    stub::function<void(uint8_t*)> write_nalu_header;
    stub::function<uint32_t()> nalu_header_size;

    stub::function<uint32_t()> nalu_count;
    stub::function<const uint8_t*(uint32_t)> nalu_at;
    stub::function<uint32_t(uint32_t)> nalu_size_at;

};

using dummy_stack = petro::extractor::annex_b_writer_layer<dummy_layer>;
}

TEST(extractor_annex_b_writer_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    layer.nalu_count.set_return(2U);
    layer.nalu_header_size.set_return(4U);
    layer.nalu_size_at.set_return(3U);

    auto expected_annex_b_size = 2U * (4U + 3U);

    EXPECT_EQ(expected_annex_b_size, stack.annex_b_size());

    std::vector<uint8_t> expected_annex_b =
        {
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
        };

    std::vector<uint8_t> nalu = {0x01, 0x02, 0x03};
    layer.nalu_at.set_return(nalu.data());

    std::vector<uint8_t> annex_b(stack.annex_b_size());
    ASSERT_EQ(expected_annex_b.size(), stack.annex_b_size());
    stack.write_annex_b(annex_b.data());
    EXPECT_EQ(expected_annex_b, annex_b);
    EXPECT_TRUE(layer.write_nalu_header.expect_calls()
                .with(annex_b.data())
                .with(annex_b.data() + 7)
                .to_bool());
}