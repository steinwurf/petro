// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/timestamp_extractor_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{

struct dummy_trak
{
    template<class Child>
    std::shared_ptr<const Child> get_child() const
    {
        if (Child::TYPE == petro::box::stts::TYPE)
            return std::dynamic_pointer_cast<const Child>(m_stts);
        if (Child::TYPE == petro::box::mdhd::TYPE)
            return std::dynamic_pointer_cast<const Child>(m_mdhd);
        return nullptr;
    }

    std::shared_ptr<const petro::box::stts> m_stts;
    std::shared_ptr<const petro::box::mdhd> m_mdhd;
};

struct dummy_layer
{
    stub::function<void(std::error_code)> open;
    stub::function<void()> close;
    stub::function<const dummy_trak*()> trak;
    stub::function<const uint8_t* ()> data;
    stub::function<uint32_t()> sample_index;
};

using dummy_stack =
    petro::extractor::timestamp_extractor_layer<dummy_layer>;
}

TEST(extractor_test_timestamp_extractor_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    dummy_trak trak;

    //-------------//
    // create stts //
    //-------------//

    // This buffer is made up, to create a dummy stts box. please see
    // stts.hpp for information related to this.
    std::vector<uint8_t> stts_buffer =
        {
            0x00, 0x00, 0x00, 0x18, // box size
            's', 't', 't', 's', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x01, // stts entry count 1
            0x00, 0x00, 0x00, 0x03, // stts entry sample_count
            0x00, 0x00, 0x00, 0x06, // stts entry sample_delta
        };

    auto stts = std::make_shared<petro::box::stts>(
        stts_buffer.data(), stts_buffer.size());

    std::error_code error;
    stts->parse(error);
    ASSERT_FALSE(bool(error));

    trak.m_stts = stts;

    //-------------//
    // create mdhd //
    //-------------//

    // This buffer is made up, to create a dummy mdhd box. please see
    // mdhd.hpp for information related to this.
    std::vector<uint8_t> mdhd_buffer =
        {
            0x00, 0x00, 0x00, 0x20, // box size
            'm', 'd', 'h', 'd', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x00, // mdhd m_creation_time
            0x00, 0x00, 0x00, 0x00, // mdhd m_modification_time
            0x00, 0x00, 0x00, 0x42, // mdhd m_timescale
            0x00, 0x00, 0x00, 0x00, // mdhd m_duration
            0x00, 0x00, // mdhd langauge code
            0x00, 0x00  // mdhd predefined
        };

    auto mdhd = std::make_shared<petro::box::mdhd>(
        mdhd_buffer.data(), mdhd_buffer.size());

    mdhd->parse(error);
    ASSERT_FALSE(bool(error));

    trak.m_mdhd = mdhd;

    layer.trak.set_return(&trak);
    layer.sample_index.set_return(0);

    // test stack
    stack.open(error);
    ASSERT_FALSE(bool(error));

    // The value is not verified. This is merely a test that checks for
    // consistency - not correctness.
    EXPECT_EQ(90909U, stack.decoding_timestamp());
    EXPECT_EQ(90909U, stack.presentation_timestamp());

    stack.close();
    EXPECT_EQ(1U, layer.close.calls());
}
