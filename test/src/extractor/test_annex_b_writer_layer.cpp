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
        stub::function<const uint8_t*()> sample_data;
        stub::function<uint32_t()> sample_size;
    };

    using dummy_stack = petro::extractor::annex_b_writer_layer<dummy_layer>;
}

TEST(extractor_annex_b_writer_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;
    layer.open.set_return(false, true);
    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());
}