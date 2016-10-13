// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/timestamp_extractor_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_root
{
    template<class Child>
    std::shared_ptr<const Child> get_child() const
    {
        if (Child::TYPE == petro::box::mvhd::TYPE)
            return std::dynamic_pointer_cast<const Child>(m_mvhd);
        return nullptr;
    }

    std::shared_ptr<const petro::box::mvhd> m_mvhd;
};

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
    stub::function<const dummy_root*()> root;
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

    layer.open.set_return(false, true);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());

    dummy_trak trak;

    //-------------//
    // create stts //
    //-------------//

    // This buffer is made up, to create a dummy stts box. please see
    // stts.hpp for information related to this.
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

    // This buffer is made up, to create a dummy mdhd box. please see
    // mdhd.hpp for information related to this.
    std::vector<uint8_t> mdhd_buffer =
        {
            // These values have already been read by the parser:
            // 0x00, 0x00, 0x00, 0xXX, // box size
            // 'm', 'd', 'h', 'd', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x00, // mdhd m_creation_time
            0x00, 0x00, 0x00, 0x00, // mdhd m_modification_time
            0x00, 0x00, 0x00, 0x42, // mdhd m_timescale
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

    dummy_root root;

    //-------------//
    // create mvhd //
    //-------------//

    // This buffer is made up, to create a dummy mvhd box. please see
    // mvhd.hpp for information related to this.
    std::vector<uint8_t> mvhd_buffer =
        {
            // These values have already been read by the parser:
            // 0x00, 0x00, 0x00, 0xXX, // box size
            // 'm', 'v', 'h', 'd', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x00, // mvhd m_creation_time
            0x00, 0x00, 0x00, 0x00, // mvhd m_modification_time
            0x00, 0x00, 0x00, 0x02, // mvhd m_timescale
            0x00, 0x00, 0x00, 0x06, // mvhd m_duration
            0x00, 0x00, 0x00, 0x00, // mvhd m_rate
            0x00, 0x00  // mvhd m_volume
        };
    // Extend the buffer with zeroes to reach the full size of the mvhd box
    mvhd_buffer.resize(mvhd_buffer.size() + 10 + 4 * 9 + 6 * 4 + 4);
    // size including attributes read by parser:
    auto mvhd_size = mvhd_buffer.size() + 8;

    petro::byte_stream mvhd_byte_stream(mvhd_buffer.data(), mvhd_buffer.size());
    auto mvhd =
        std::make_shared<petro::box::mvhd>(std::weak_ptr<petro::box::box>());
    mvhd->read(mvhd_size, mvhd_byte_stream);

    root.m_mvhd = mvhd;

    layer.trak.set_return(&trak);
    layer.root.set_return(&root);
    layer.sample_index.set_return(0);

    // test stack

    EXPECT_TRUE(stack.open());

    // The value is not verified. This is merely a test that checks for
    // consistency - not correctness.
    EXPECT_EQ(90909U, stack.decoding_timestamp());
    EXPECT_EQ(90909U, stack.presentation_timestamp());
    EXPECT_EQ(3000000U, stack.video_length());

    stack.close();
    EXPECT_EQ(2U, layer.close.calls());
}
