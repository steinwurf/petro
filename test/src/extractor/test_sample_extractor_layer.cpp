// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/sample_extractor_layer.hpp>

#include <petro/box/stsc.hpp>
#include <petro/box/stsz.hpp>
#include <petro/box/co64.hpp>
#include <petro/box/data_box.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_trak : public petro::box::box
{
    std::string type() const override
    {
        return "trak";
    }

    std::string describe() const override
    {
        return "";
    }
};

struct dummy_layer
{
    stub::function<void(std::error_code)> open;
    stub::function<void()> close;
    stub::function<std::shared_ptr<const petro::box::box>()> trak;
    stub::function<const uint8_t* ()> data;
};

using dummy_stack = petro::extractor::sample_extractor_layer<dummy_layer>;
}

TEST(extractor_test_sample_extractor_layer, init)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    auto trak = std::make_shared<dummy_trak>();

    //-------------//
    // create stsc //
    //-------------//

    // This buffer is made up, to create a dummy stsc box. please see
    // stsc.hpp for information related to this.
    std::vector<uint8_t> stsc_buffer =
        {
            0x00, 0x00, 0x00, 0x1C, // box size
            's', 't', 's', 'c', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x01, // stsc entry count 1
            0x00, 0x00, 0x00, 0x01, // stsc entry first_chunk
            0x00, 0x00, 0x00, 0x01, // stsc entry samples_per_chunk
            0x00, 0x00, 0x00, 0x01, // stsc entry sample_description_index
        };

    auto stsc = std::make_shared<petro::box::stsc>(
        stsc_buffer.data(), stsc_buffer.size());

    std::error_code error;
    stsc->parse(error);
    ASSERT_FALSE(bool(error));

    trak->add_child(stsc);

    //-------------//
    // create stsz //
    //-------------//

    uint8_t sample_size = 42;
    // This buffer is made up, to create a dummy stsz box. please see
    // stsz.hpp for information related to this.
    std::vector<uint8_t> stsz_buffer =
        {
            0x00, 0x00, 0x00, 0x14, // box size
            's', 't', 's', 'z', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, sample_size, // stsz sample size
            0x00, 0x00, 0x00, 0x02, // stsz sample count
        };

    auto stsz = std::make_shared<petro::box::stsz>(
        stsz_buffer.data(), stsz_buffer.size());

    stsz->parse(error);
    ASSERT_FALSE(error);

    trak->add_child(stsz);

    //-------------//
    // create co64 //
    //-------------//

    // This buffer is made up, to create a dummy co64 box. please see
    // co64.hpp for information related to this.
    uint8_t chunk_offset1 = 1U;
    uint8_t chunk_offset2 = 2U;
    std::vector<uint8_t> co64_buffer =
        {
            0x00, 0x00, 0x00, 0x20, // box size
            'c', 'o', '6', '4', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x02, // co64 entry count
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, chunk_offset1, // entry 1
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, chunk_offset2  // entry 2
        };

    auto co64 = std::make_shared<petro::box::co64>(
        co64_buffer.data(), co64_buffer.size());
    co64->parse(error);
    ASSERT_FALSE(bool(error));

    trak->add_child(co64);

    layer.trak.set_return(trak);

    // Do the test

    stack.open(error);
    ASSERT_FALSE(bool(error));
    EXPECT_FALSE(stack.at_end());

    auto data = (const uint8_t*)0x00000042;
    layer.data.set_return(data);

    EXPECT_EQ(data + chunk_offset1, stack.sample_data());
    EXPECT_EQ(sample_size, stack.sample_size());
    EXPECT_EQ(0U, stack.sample_index());
    stack.advance();
    EXPECT_FALSE(stack.at_end());
    EXPECT_EQ(data + chunk_offset2, stack.sample_data());
    EXPECT_EQ(sample_size, stack.sample_size());
    EXPECT_EQ(1U, stack.sample_index());

    stack.advance();
    EXPECT_TRUE(stack.at_end());

    // Reset stack and verfiy the results are still the same.
    stack.reset();
    EXPECT_EQ(data + chunk_offset1, stack.sample_data());
    EXPECT_EQ(sample_size, stack.sample_size());
    EXPECT_EQ(0U, stack.sample_index());
    stack.advance();
    EXPECT_FALSE(stack.at_end());
    EXPECT_EQ(data + chunk_offset2, stack.sample_data());
    EXPECT_EQ(sample_size, stack.sample_size());
    EXPECT_EQ(1U, stack.sample_index());

    stack.advance();
    EXPECT_TRUE(stack.at_end());
}
