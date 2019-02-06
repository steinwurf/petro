// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/media_duration_layer.hpp>

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

struct dummy_layer
{
    stub::function<void(std::error_code)> open;
    stub::function<void()> close;
    stub::function<const dummy_root*()> root;
};

using dummy_stack =
    petro::extractor::media_duration_layer<dummy_layer>;
}

TEST(extractor_test_media_duration_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;
    dummy_root root;

    //-------------//
    // create mvhd //
    //-------------//

    // This buffer is made up, to create a dummy mvhd box. please see
    // mvhd.hpp for information related to this.
    std::vector<uint8_t> mvhd_buffer =
        {
            0x00, 0x00, 0x00, 0x6C, // box size
            'm', 'v', 'h', 'd', // box type
            0x00, // full_box version
            0x00, 0x00, 0x00, // full_box flag
            0x00, 0x00, 0x00, 0x00, // mvhd m_creation_time
            0x00, 0x00, 0x00, 0x00, // mvhd m_modification_time
            0x00, 0x00, 0x00, 0x02, // mvhd m_timescale
            0x00, 0x00, 0x00, 0x06, // mvhd m_duration
            0x00, 0x00, 0x00, 0x00, // mvhd m_rate
            0x00, 0x00,  // mvhd m_volume
            /// remaining unused data
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

    auto mvhd = std::make_shared<petro::box::mvhd>(
        mvhd_buffer.data(), mvhd_buffer.size());

    std::error_code error;
    mvhd->parse(error);
    ASSERT_FALSE(bool(error));

    root.m_mvhd = mvhd;
    layer.root.set_return(&root);

    // test stack
    stack.open(error);
    ASSERT_FALSE(bool(error));

    // The value is not verified. This is merely a test that checks for
    // consistency - not correctness.
    EXPECT_EQ(3000000U, stack.media_duration());

    stack.close();
    EXPECT_EQ(1U, layer.close.calls());
}
