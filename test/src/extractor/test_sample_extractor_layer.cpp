// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/sample_extractor_layer.hpp>

#include <petro/box/stsc.hpp>
#include <petro/box/stsz.hpp>
#include <petro/box/co64.hpp>
#include <petro/box/box.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_trak : public petro::box::box
{
    dummy_trak() :
        petro::box::box("dummy_trak", std::weak_ptr<box>())
    { }
};

struct dummy_layer
{
    stub::function<bool()> open;
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

    layer.open.set_return(false, true);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    auto trak = std::make_shared<dummy_trak>();

    //-------------//
    // create stsc //
    //-------------//

    // This buffer is made up, to create a dummy stsc box. please see
    // stsc.hpp for information related to this.
    std::vector<uint8_t> stsc_buffer =
        {
            // These values have already been read by the parser:
            // 0x00, 0x00, 0x00, 0xXX, // box size
            // 's', 't', 's', 'c', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x01, // stsc entry count 1
            0x00, 0x00, 0x00, 0x01, // stsc entry first_chunk
            0x00, 0x00, 0x00, 0x01, // stsc entry samples_per_chunk
            0x00, 0x00, 0x00, 0x01, // stsc entry sample_description_index
        };
    // size including attributes read by parser:
    auto stsc_size = stsc_buffer.size() + 8;

    petro::byte_stream stsc_byte_stream(stsc_buffer.data(), stsc_buffer.size());
    auto stsc =
        std::make_shared<petro::box::stsc>(std::weak_ptr<petro::box::box>());
    stsc->read(stsc_size, stsc_byte_stream);

    trak->add_child(stsc);

    //-------------//
    // create stsz //
    //-------------//

    uint8_t sample_size = 42;
    // This buffer is made up, to create a dummy stsz box. please see
    // stsz.hpp for information related to this.
    std::vector<uint8_t> stsz_buffer =
        {
            // These values have already been read by the parser:
            // 0x00, 0x00, 0x00, 0xXX, // box size
            // 's', 't', 's', 'z', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, sample_size, // stsz sample size
            0x00, 0x00, 0x00, 0x02, // stsz sample count
        };
    // size including attributes read by parser:
    auto stsz_size = stsz_buffer.size() + 8;

    petro::byte_stream stsz_byte_stream(stsz_buffer.data(), stsz_buffer.size());
    auto stsz =
        std::make_shared<petro::box::stsz>(std::weak_ptr<petro::box::box>());
    stsz->read(stsz_size, stsz_byte_stream);

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
            // These values have already been read by the parser:
            // 0x00, 0x00, 0x00, 0xXX, // box size
            // 'c', 'o', '6', '4', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x02, // co64 entry count
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, chunk_offset1, // entry 1
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, chunk_offset2  // entry 2
        };
    // size including attributes read by parser:
    auto co64_size = co64_buffer.size() + 8;

    petro::byte_stream co64_byte_stream(co64_buffer.data(), co64_buffer.size());
    auto co64 =
        std::make_shared<petro::box::co64>(std::weak_ptr<petro::box::box>());
    co64->read(co64_size, co64_byte_stream);

    trak->add_child(co64);

    layer.trak.set_return(trak);

    // Do the test

    EXPECT_TRUE(stack.open());
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
