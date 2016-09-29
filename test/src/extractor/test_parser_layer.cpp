// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/parser_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_layer
{
    stub::function<bool()> open;
    stub::function<void()> close;
    stub::function<const uint8_t* ()> data;
    stub::function<uint32_t()> data_size;
};

using dummy_stack = petro::extractor::parser_layer<dummy_layer>;
}

TEST(extractor_test_parser_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    layer.open.set_return(false, true);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    std::vector<uint8_t> data =
        {
            0x00, 0x00, 0x00, 0x09, 'b', 'a', 'b', 'y', 0x42
        };

    layer.data.set_return(data.data());
    layer.data_size.set_return(data.size());

    EXPECT_TRUE(stack.open());
    auto root = stack.root();
    EXPECT_EQ(nullptr, root->get_child("not1"));
    EXPECT_NE(nullptr, root->get_child("baby"));
    EXPECT_NE(nullptr, root);

    stack.close();
    EXPECT_EQ(2U, layer.close.calls());
}
