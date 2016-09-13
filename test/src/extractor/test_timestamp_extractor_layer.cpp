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
        stub::function<bool()> open;
        stub::function<void()> close;
        stub::function<const dummy_trak*()> trak;
        stub::function<const uint8_t*()> data;
        stub::function<uint32_t()> sample_index;
    };

    using dummy_stack =
        petro::extractor::timestamp_extractor_layer<dummy_layer>;
}

TEST(extractor_test_timestamp_extractor_layer, init)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    layer.open.set_return(false, true);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    dummy_trak trak;

    //-------------//
    // create stts //
    //-------------//

    std::vector<uint8_t> stts_buffer =
    {
        // These values have already been read by the parser:
        // 0x00, 0x00, 0x00, 0xXX, // box size
        // 's', 't', 't', 's', // box type
        0x00, // full_box version
        0x00, 0x00, 0x00, // full_box flag
        0x00, 0x00, 0x00, 0x01, // stts entry count 1
        0x00, 0x00, 0x00, 0x03, // stts entry sample_count
        0x00, 0x00, 0x00, 0x06, // stts entry sample_delta
    };
    // size including attributes read by parser:
    auto stts_size = stts_buffer.size() + 8;

    petro::byte_stream stts_byte_stream(stts_buffer.data(), stts_buffer.size());
    auto stts =
        std::make_shared<petro::box::stts>(std::weak_ptr<petro::box::box>());
    stts->read(stts_size, stts_byte_stream);

    trak.m_stts = stts;

    //-------------//
    // create mdhd //
    //-------------//

    uint8_t timescale = 42;
    std::vector<uint8_t> mdhd_buffer =
    {
        // These values have already been read by the parser:
        // 0x00, 0x00, 0x00, 0xXX, // box size
        // 'm', 'd', 'h', 'd', // box type
        0x00, // full_box version
        0x00, 0x00, 0x00, // full_box flag
        0x00, 0x00, 0x00, 0x00, // mdhd m_creation_time
        0x00, 0x00, 0x00, 0x00, // mdhd m_modification_time
        0x00, 0x00, 0x00, timescale, // mdhd m_timescale
        0x00, 0x00, 0x00, 0x00, // mdhd m_duration
        0x00, 0x00, // mdhd langauge code
        0x00, 0x00  // mdhd predefined
    };
    // size including attributes read by parser:
    auto mdhd_size = mdhd_buffer.size() + 8;

    petro::byte_stream mdhd_byte_stream(mdhd_buffer.data(), mdhd_buffer.size());
    auto mdhd =
        std::make_shared<petro::box::mdhd>(std::weak_ptr<petro::box::box>());
    mdhd->read(mdhd_size, mdhd_byte_stream);

    trak.m_mdhd = mdhd;

    layer.trak.set_return(&trak);
    layer.sample_index.set_return(0);

    // test stack

    EXPECT_TRUE(stack.open());

    // The value is not verified. This is merly a test that checks for
    // consistency - not correctness.
    EXPECT_EQ(142857U, stack.decoding_timestamp());
    EXPECT_EQ(142857U, stack.presentation_timestamp());
    EXPECT_EQ(142857U, stack.timestamp());

    stack.close();
    EXPECT_EQ(2U, layer.close.calls());
}
