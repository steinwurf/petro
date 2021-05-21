// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/data_layer.hpp>
#include <petro/extractor/parser_layer.hpp>
#include <petro/extractor/sample_extractor_layer.hpp>
#include <petro/extractor/timestamp_extractor_layer.hpp>
#include <petro/extractor/track_layer.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{

struct dummy_trak
{
    template <class Child>
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
    stub::function<const uint8_t*()> data;
    stub::function<uint32_t()> sample_index;
};

using dummy_stack = petro::extractor::timestamp_extractor_layer<dummy_layer>;
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
    std::vector<uint8_t> stts_buffer = {
        0x00, 0x00, 0x00, 0x18, // box size
        's',  't',  't',  's',  // box type
        0x00,                   // full_box version
        0x00, 0x00, 0x00,       // full_box flag
        0x00, 0x00, 0x00, 0x01, // stts entry count 1
        0x00, 0x00, 0x00, 0x03, // stts entry sample_count
        0x00, 0x00, 0x00, 0x06, // stts entry sample_delta
    };

    auto stts = std::make_shared<petro::box::stts>(stts_buffer.data(),
                                                   stts_buffer.size());

    std::error_code error;
    stts->parse(error);
    ASSERT_FALSE(bool(error));

    trak.m_stts = stts;

    //-------------//
    // create mdhd //
    //-------------//

    // This buffer is made up, to create a dummy mdhd box. please see
    // mdhd.hpp for information related to this.
    std::vector<uint8_t> mdhd_buffer = {
        0x00, 0x00, 0x00, 0x20, // box size
        'm',  'd',  'h',  'd',  // box type
        0x00,                   // full_box version
        0x00, 0x00, 0x00,       // full_box flag
        0x00, 0x00, 0x00, 0x00, // mdhd m_creation_time
        0x00, 0x00, 0x00, 0x00, // mdhd m_modification_time
        0x00, 0x00, 0x00, 0x42, // mdhd m_timescale
        0x00, 0x00, 0x00, 0x00, // mdhd m_duration
        0x00, 0x00,             // mdhd langauge code
        0x00, 0x00              // mdhd predefined
    };

    auto mdhd = std::make_shared<petro::box::mdhd>(mdhd_buffer.data(),
                                                   mdhd_buffer.size());

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

namespace
{
using timestamp_extractor = petro::extractor::timestamp_extractor_layer<
    petro::extractor::sample_extractor_layer<petro::extractor::track_layer<
        petro::extractor::parser_layer<petro::extractor::data_layer>>>>;

void test_timestamps(timestamp_extractor& extractor,
                     const std::vector<uint64_t>& presentation_timestamps,
                     const std::vector<uint64_t>& decoding_timestamps)
{
    for (auto presentation_timestamp : presentation_timestamps)
    {
        ASSERT_FALSE(extractor.at_end());
        EXPECT_EQ(presentation_timestamp, extractor.presentation_timestamp());
        extractor.advance();
    }
    EXPECT_TRUE(extractor.at_end());
    extractor.reset();
    EXPECT_EQ(presentation_timestamps.at(0),
              extractor.presentation_timestamp());

    for (auto decoding_timestamp : decoding_timestamps)
    {
        ASSERT_FALSE(extractor.at_end());
        EXPECT_EQ(decoding_timestamp, extractor.decoding_timestamp());
        extractor.advance();
    }
    extractor.reset();
    EXPECT_EQ(decoding_timestamps.at(0), extractor.decoding_timestamp());
}

}

TEST(extractor_test_timestamp_extractor_layer, output_test1)
{
    boost::iostreams::mapped_file_source file;
    file.open("test1.mp4");

    timestamp_extractor extractor;
    extractor.set_track_id(1);
    extractor.set_buffer((uint8_t*)file.data(), file.size());

    std::error_code error;
    extractor.open(error);
    ASSERT_FALSE(bool(error));
    std::vector<uint64_t> expected_timestamps = {
        33333,   66666,   100000,  133333,  166666,  200000,  233333,  266666,
        300000,  333333,  366666,  400000,  433333,  466666,  500000,  533333,
        566666,  600000,  633333,  666666,  700000,  733333,  766666,  800000,
        833333,  866666,  900000,  933333,  966666,  1000000, 1033333, 1066666,
        1100000, 1133333, 1166666, 1200000, 1233333, 1266666, 1300000, 1333333,
        1366666, 1400000, 1433333, 1466666, 1500000, 1533333, 1566666, 1600000,
        1633333, 1666666, 1700000, 1733333, 1766666, 1800000, 1833333, 1866666,
        1900000, 1933333, 1966666, 2000000, 2033333, 2066666, 2100000, 2133333,
        2166666, 2200000, 2233333, 2266666, 2300000, 2333333, 2366666, 2400000,
        2433333, 2466666, 2500000, 2533333, 2566666, 2600000, 2633333, 2666666,
        2700000, 2733333, 2766666, 2800000, 2833333, 2866666, 2900000, 2933333,
        2966666, 3000000, 3033333, 3066666, 3100000, 3133333, 3166666, 3200000,
        3233333, 3266666, 3300000, 3333333, 3366666, 3400000, 3433333, 3466666,
        3500000, 3533333, 3566666, 3600000, 3633333, 3666666, 3700000, 3733333,
        3766666, 3800000, 3833333, 3866666, 3900000, 3933333, 3966666, 4000000,
        4033333, 4066666, 4100000, 4133333, 4166666, 4200000, 4233333, 4266666,
        4300000, 4333333, 4366666, 4400000, 4433333, 4466666, 4500000, 4533333,
        4566666, 4600000, 4633333, 4666666, 4700000, 4733333, 4766666, 4800000,
        4833333, 4866666, 4900000, 4933333, 4966666, 5000000, 5033333, 5066666,
        5100000, 5133333, 5166666, 5200000, 5233333, 5266666, 5300000, 5333333,
        5366666, 5400000, 5433333, 5466666, 5500000, 5533333};

    // Note: The fact that the presentation time and decoding time is the
    // same is coincidental.
    test_timestamps(extractor, expected_timestamps, expected_timestamps);
}

TEST(extractor_test_timestamp_extractor_layer, output_test3)
{
    boost::iostreams::mapped_file_source file;
    file.open("test3.mp4");

    timestamp_extractor extractor;
    extractor.set_track_id(1);
    extractor.set_buffer((uint8_t*)file.data(), file.size());
    std::error_code error;
    extractor.open(error);
    ASSERT_FALSE(bool(error));

    std::vector<uint64_t> expected_presentation_timestamps = {
        124999,  166666,  208332,  250000,  291666,  333332,  375000,  416666,
        458332,  500000,  541666,  583332,  625000,  666666,  708332,  750000,
        791666,  833332,  875000,  916666,  958332,  1000000, 1041666, 1083332,
        1125000, 1166666, 1208332, 1250000, 1291666, 1333332, 1375000, 1416666,
        1458332, 1500000, 1541666, 1583332, 1625000, 1666666, 1708332, 1750000,
        1791666, 1833332, 1875000, 1916666, 1958332, 2000000, 2041666, 2083332,
        2125000, 2166666, 2208332, 2250000, 2291666, 2333332, 2375000, 2416666,
        2458332, 2500000, 2541666, 2583332, 2625000, 2666666, 2708332, 2750000,
        2791666, 2833332, 2875000, 2916666, 2958332, 3000000, 3041666, 3083332,
        3125000, 3166666, 3208332, 3250000, 3291666, 3333332, 3375000, 3416666,
        3458332, 3500000, 3541666, 3583332, 3625000, 3666666, 3708332, 3750000,
        3791666, 3833332, 3875000, 3916666, 3958332, 4000000, 4041666, 4083332,
        4125000, 4166666, 4208332, 4250000, 4291666, 4333332, 4375000, 4416666,
        4458332, 4500000, 4541666, 4583332, 4625000, 4666666, 4708332, 4750000,
        4791666, 4833332, 4875000, 4916666, 4958332, 5000000, 5041666, 5083332,
        5125000, 5166666, 5208332, 5250000, 5291666, 5333332, 5375000, 5416666,
        5458332, 5500000, 5541666, 5583332, 5625000, 5666666, 5708332, 5750000,
        5791666, 5833332, 5875000, 5916666, 5958332, 6000000, 6041666, 6083332,
        6125000, 6166666, 6208332, 6250000};

    std::vector<uint64_t> expected_decoding_timestamps = {
        41666,   83333,   124999,  166666,  208333,  249999,  291666,  333333,
        374999,  416666,  458333,  499999,  541666,  583333,  624999,  666666,
        708333,  749999,  791666,  833333,  874999,  916666,  958333,  999999,
        1041666, 1083333, 1124999, 1166666, 1208333, 1249999, 1291666, 1333333,
        1374999, 1416666, 1458333, 1499999, 1541666, 1583333, 1624999, 1666666,
        1708333, 1749999, 1791666, 1833333, 1874999, 1916666, 1958333, 1999999,
        2041666, 2083333, 2124999, 2166666, 2208333, 2249999, 2291666, 2333333,
        2374999, 2416666, 2458333, 2499999, 2541666, 2583333, 2624999, 2666666,
        2708333, 2749999, 2791666, 2833333, 2874999, 2916666, 2958333, 2999999,
        3041666, 3083333, 3124999, 3166666, 3208333, 3249999, 3291666, 3333333,
        3374999, 3416666, 3458333, 3499999, 3541666, 3583333, 3624999, 3666666,
        3708333, 3749999, 3791666, 3833333, 3874999, 3916666, 3958333, 3999999,
        4041666, 4083333, 4124999, 4166666, 4208333, 4249999, 4291666, 4333333,
        4374999, 4416666, 4458333, 4499999, 4541666, 4583333, 4624999, 4666666,
        4708333, 4749999, 4791666, 4833333, 4874999, 4916666, 4958333, 4999999,
        5041666, 5083333, 5124999, 5166666, 5208333, 5249999, 5291666, 5333333,
        5374999, 5416666, 5458333, 5499999, 5541666, 5583333, 5624999, 5666666,
        5708333, 5749999, 5791666, 5833333, 5874999, 5916666, 5958333, 5999999,
        6041666, 6083333, 6124999, 6166666,
    };

    test_timestamps(extractor, expected_presentation_timestamps,
                    expected_decoding_timestamps);
}