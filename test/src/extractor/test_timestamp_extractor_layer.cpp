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

    // dummy_trak trak;

    // // stts

    // std::vector<uint8_t> stts_buffer =
    // {
    //     // These values have already been read by the parser:
    //     // 0x00, 0x00, 0x00, 0xXX, // box size
    //     // 's', 't', 's', 'c', // box type
    //     0x01, // full_box version
    //     0x00, 0x00, 0x00, // full_box flag
    //     0x00, 0x00, 0x00, 0x01, // stts entry count 1
    //     0x00, 0x00, 0x00, 0x01, // stts entry first_chunk
    //     0x00, 0x00, 0x00, 0x01, // stts entry samples_per_chunk
    //     0x00, 0x00, 0x00, 0x01, // stts entry sample_description_index
    // };
    // // size including attributes read by parser:
    // auto stts_size = stts_buffer.size() + 8;

    // petro::byte_stream stts_byte_stream(stts_buffer.data(), stts_buffer.size());
    // auto stts =
    //     std::make_shared<petro::box::stts>(std::weak_ptr<petro::box::box>());
    // stts->read(stts_size, stts_byte_stream);

    // trak.m_stts = stts;

    // // mdhd
    // uint8_t sample_size = 42;
    // std::vector<uint8_t> mdhd_buffer =
    // {
    //     // These values have already been read by the parser:
    //     // 0x00, 0x00, 0x00, 0xXX, // box size
    //     // 's', 't', 's', 'z', // box type
    //     0x01, // full_box version
    //     0x00, 0x00, 0x00, // full_box flag
    //     0x00, 0x00, 0x00, sample_size, // mdhd sample size
    //     0x00, 0x00, 0x00, 0x02, // mdhd sample count
    // };
    // // size including attributes read by parser:
    // auto mdhd_size = mdhd_buffer.size() + 8;

    // petro::byte_stream mdhd_byte_stream(mdhd_buffer.data(), mdhd_buffer.size());
    // auto mdhd =
    //     std::make_shared<petro::box::mdhd>(std::weak_ptr<petro::box::box>());
    // mdhd->read(mdhd_size, mdhd_byte_stream);

    // trak.m_mdhd = mdhd;

    // layer.trak.set_return(&trak);

}
