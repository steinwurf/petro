// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/avc_sample_access_layer.hpp>

#include <stub/function.hpp>
#include <gtest/gtest.h>

namespace
{
struct dummy_layer
{
    stub::function<void(std::error_code)> open;
    stub::function<void()> close;
    stub::function<void()> reset;
    stub::function<void()> advance;
    stub::function<bool()> at_end;
    stub::function<uint32_t()> nalu_length_size;
    stub::function<const uint8_t* ()> sample_data;
    stub::function<uint32_t()> sample_size;
};

using dummy_stack = petro::extractor::avc_sample_access_layer<dummy_layer>;
}

TEST(extractor_avc_sample_access_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    std::vector<uint8_t> avc_sample
    {
        0x00, 0x00, 0x03, 0x22, 0x33, 0x44,
        0x00, 0x00, 0x04, 0x77, 0x88, 0x99, 0x13,
        0x00, 0x00, 0x02, 0x74, 0x34
    };

    layer.at_end.set_return(false);
    layer.nalu_length_size.set_return(3U);
    layer.sample_data.set_return(avc_sample.data());
    layer.sample_size.set_return(avc_sample.size());

    std::error_code error;
    stack.open(error);
    ASSERT_FALSE(bool(error));

    ASSERT_EQ(3U, stack.nalu_count());

    {
        std::vector<uint8_t> expected_nalu = {0x22, 0x33, 0x44};
        ASSERT_EQ(stack.nalu_size_at(0), expected_nalu.size());
        std::vector<uint8_t> nalu(
            stack.nalu_at(0), stack.nalu_at(0) + stack.nalu_size_at(0));
        ASSERT_EQ(expected_nalu, nalu);
    }

    {
        std::vector<uint8_t> expected_nalu = {0x77, 0x88, 0x99, 0x13};
        ASSERT_EQ(stack.nalu_size_at(1), expected_nalu.size());
        std::vector<uint8_t> nalu(
            stack.nalu_at(1), stack.nalu_at(1) + stack.nalu_size_at(1));
        ASSERT_EQ(expected_nalu, nalu);
    }

    {
        std::vector<uint8_t> expected_nalu = {0x74, 0x34};
        ASSERT_EQ(stack.nalu_size_at(2), expected_nalu.size());
        std::vector<uint8_t> nalu(
            stack.nalu_at(2), stack.nalu_at(2) + stack.nalu_size_at(2));
        ASSERT_EQ(expected_nalu, nalu);
    }

    layer.at_end.set_return(false);
    stack.advance();
}
