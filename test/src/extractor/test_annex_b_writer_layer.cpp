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
    stub::function<bool()> open;
    stub::function<void()> close;
    stub::function<void()> reset;
    stub::function<void()> advance;
    stub::function<void(uint8_t*)> write_nalu_header;
    stub::function<uint32_t()> nalu_header_size;
    stub::function<bool()> at_end;
    stub::function<uint32_t()> nalu_length_size;
    stub::function<const uint8_t* ()> sample_data;
    stub::function<uint32_t()> sample_size;
};

using dummy_stack = petro::extractor::annex_b_writer_layer<dummy_layer>;
}

TEST(extractor_annex_b_writer_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;
    layer.open.set_return(false);
    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    std::vector<uint8_t> avc_sample
    {
        0x00, 0x00, 0x03, 0x01, 0x02, 0x03,
        0x00, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
        0x00, 0x00, 0x02, 0x01, 0x02
    };

    layer.at_end.set_return(false);
    layer.nalu_length_size.set_return(3U);
    layer.sample_data.set_return(avc_sample.data());
    layer.sample_size.set_return(avc_sample.size());

    layer.nalu_header_size.set_return(4U);

    layer.open.set_return(true);
    EXPECT_TRUE(stack.open());
    auto expected_annex_b_size = 9U + 3U * 4U;

    EXPECT_EQ(expected_annex_b_size, stack.annex_b_size());

    std::vector<uint8_t> expected_annex_b =
        {
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02
        };

    std::vector<uint8_t> annex_b(stack.annex_b_size());
    stack.write_annex_b(annex_b.data());
    EXPECT_EQ(expected_annex_b, annex_b);
    EXPECT_TRUE(layer.write_nalu_header.expect_calls()
                .with(annex_b.data())
                .with(annex_b.data() + 7)
                .with(annex_b.data() + 7 + 8)
                .to_bool());

    layer.at_end.set_return(true);
    stack.advance();
}