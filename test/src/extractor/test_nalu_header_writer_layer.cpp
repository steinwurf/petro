// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/nalu_header_writer_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
    struct dummy_layer
    { };

    using dummy_stack = petro::extractor::nalu_header_writer_layer<dummy_layer>;
}
TEST(extractor_test_nalu_header_writer_layer, init)
{
    dummy_stack stack;
    std::vector<uint8_t> header(stack.nalu_header_size());
    stack.write_nalu_header(header.data());

    std::vector<uint8_t> expected_header = { 0x00, 0x00, 0x00, 0x01 };
    EXPECT_EQ(expected_header, header);
}
