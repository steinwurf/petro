// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/adts_writer_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_layer
{
    stub::function<uint8_t()> channel_configuration;
    stub::function<uint8_t()> mpeg_audio_object_type;
    stub::function<uint32_t()> frequency_index;
    stub::function<uint32_t()> sample_size;
};

using dummy_stack = petro::extractor::adts_writer_layer<dummy_layer>;
}

TEST(extractor_test_adts_writer_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    EXPECT_EQ(7U, stack.adts_header_size());
    std::vector<uint8_t> header(stack.adts_header_size());


    layer.channel_configuration.set_return(1U);
    layer.mpeg_audio_object_type.set_return(2U);
    layer.frequency_index.set_return(3U);
    layer.sample_size.set_return(4U);

    stack.write_adts_header(header.data());
    std::vector<uint8_t> expected_header =
        {
            0xFF, 0xF1, 0x4C, 0x40, 0x01, 0x60, 0xB0
        };

    EXPECT_EQ(expected_header, header);
}