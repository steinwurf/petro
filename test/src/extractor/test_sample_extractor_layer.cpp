// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/sample_extractor_layer.hpp>

#include <petro/box/stsc.hpp>
#include <petro/box/stsz.hpp>
#include <petro/box/co64.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
    struct dummy_trak
    {
        template<class Child>
        std::shared_ptr<const Child> get_child() const
        {
            if (Child::TYPE == petro::box::stsc::TYPE)
                return std::dynamic_pointer_cast<const Child>(m_stsc);
            if (Child::TYPE == petro::box::stsz::TYPE)
                return std::dynamic_pointer_cast<const Child>(m_stsz);
            if (Child::TYPE == petro::box::co64::TYPE)
                return std::dynamic_pointer_cast<const Child>(m_co64);
            return nullptr;
        }
        std::shared_ptr<const petro::box::stsc> m_stsc;
        std::shared_ptr<const petro::box::stsz> m_stsz;
        std::shared_ptr<const petro::box::co64> m_co64;
    };

    struct dummy_layer
    {
        stub::function<bool()> open;
        stub::function<void()> close;
        stub::function<const dummy_trak*()> trak;
        stub::function<const uint8_t*()> data;
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

    dummy_trak trak;

    // stsc

    std::vector<uint8_t> stsc_buffer =
    {
        // These values have already been read by the parser:
        // 0x00, 0x00, 0x00, 0xXX, // box size
        // 's', 't', 's', 'c', // box type
        0x01, // full_box version
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

    trak.m_stsc = stsc;

    // stsz
    uint8_t sample_size = 42;
    std::vector<uint8_t> stsz_buffer =
    {
        // These values have already been read by the parser:
        // 0x00, 0x00, 0x00, 0xXX, // box size
        // 's', 't', 's', 'z', // box type
        0x01, // full_box version
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

    trak.m_stsz = stsz;

    // co64
    uint8_t chunk_offset1 = 1U;
    uint8_t chunk_offset2 = 2U;
    std::vector<uint8_t> co64_buffer =
    {
        // These values have already been read by the parser:
        // 0x00, 0x00, 0x00, 0xXX, // box size
        // 'c', 'o', '6', '4', // box type
        0x01, // full_box version
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

    trak.m_co64 = co64;

    layer.trak.set_return(&trak);

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
