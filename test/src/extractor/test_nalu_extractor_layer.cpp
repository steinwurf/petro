// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/nalu_extractor_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
    struct dummy_layer
    {
        stub::function<bool()> open;
        stub::function<void()> close;
        stub::function<bool()> at_end;
        stub::function<uint32_t()> sample_size;
        stub::function<void()> advance;
        stub::function<void()> reset;
        stub::function<const uint8_t*()> sample_data;
        stub::function<uint32_t()> nalu_length_size;
    };

    using dummy_stack = petro::extractor::nalu_extractor_layer<dummy_layer>;
}

TEST(extractor_test_nalu_extractor_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;
    layer.open.set_return(false);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    layer.open.set_return(true);
    layer.at_end.set_return(false);
    layer.nalu_length_size.set_return(4);
    std::vector<uint8_t> sample1
    {
        0x00, 0x00, 0x00, 0x01, 0xFF,
        0x00, 0x00, 0x00, 0x02, 0xBA, 0xB0
    };
    layer.sample_data.set_return(sample1.data());
    layer.sample_size.set_return(sample1.size());

    EXPECT_TRUE(stack.open());
    EXPECT_TRUE(stack.is_beginning_of_avc_sample());
    EXPECT_EQ(1U, stack.nalu_size());
    EXPECT_EQ(0xFF, stack.nalu_data()[0]);
    stack.advance();
    EXPECT_FALSE(stack.at_end());
    EXPECT_FALSE(stack.is_beginning_of_avc_sample());
    EXPECT_EQ(2U, stack.nalu_size());
    EXPECT_EQ(0xBA, stack.nalu_data()[0]);
    EXPECT_EQ(0xB0, stack.nalu_data()[1]);
    layer.at_end.set_return(true);
    EXPECT_TRUE(stack.at_end());
}
