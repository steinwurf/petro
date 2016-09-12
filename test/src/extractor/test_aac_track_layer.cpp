// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/aac_track_layer.hpp>

#include <memory>

#include <petro/box/box.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
    struct dummy_decoder_config_descriptor
    {
        uint8_t mpeg_audio_object_type() const
        {
            return 11U;
        }

        uint32_t frequency_index() const
        {
            return 22U;
        }

        uint8_t channel_configuration() const
        {
            return 33U;
        }
    };

    struct dummy_descriptor
    {
        dummy_decoder_config_descriptor* decoder_config_descriptor()
        {
            return &m_dummy_decoder_config_descriptor;
        }

        dummy_decoder_config_descriptor m_dummy_decoder_config_descriptor;
    };

    struct dummy_esds
    {
        dummy_descriptor* descriptor()
        {
            return &m_dummy_descriptor;
        }

        dummy_descriptor m_dummy_descriptor;
    };

    struct dummy_trak : public petro::box::box
    {
        dummy_trak() :
           petro::box::box("dummy_trak", std::weak_ptr<box>())
        { }
    };

    struct dummy_mp4a
    {
        dummy_mp4a() :
            m_trak(std::make_shared<dummy_trak>()),
            m_dummy_esds()
        { }

        template<class Box>
        dummy_esds* get_child()
        {
            return &m_dummy_esds;
        }

        std::shared_ptr<const petro::box::box> get_parent(
            const std::string& type)
        {
            (void) type;
            return m_trak;
        }

        std::shared_ptr<const dummy_trak> m_trak;
        dummy_esds m_dummy_esds;
    };

    struct dummy_root
    {
        dummy_root():
            m_dummy_mp4a()
        { }

        dummy_mp4a* get_child(const std::string& type)
        {
            (void) type;
            return &m_dummy_mp4a;
        }

        dummy_mp4a m_dummy_mp4a;
    };

    struct dummy_layer
    {
        stub::function<bool()> open;
        stub::function<void()> close;
        stub::function<std::shared_ptr<dummy_root>()> root;
    };

    using dummy_stack = petro::extractor::aac_track_layer<dummy_layer>;
}

TEST(extractor_test_aac_track_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;
    layer.open.set_return(false, true);
    auto root = std::make_shared<dummy_root>();
    layer.root.set_return(root);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());
    EXPECT_TRUE(stack.open());

    EXPECT_EQ(11U, stack.mpeg_audio_object_type());
    EXPECT_EQ(22U, stack.frequency_index());
    EXPECT_EQ(33U, stack.channel_configuration());
    EXPECT_EQ("dummy_trak", stack.trak()->type());

    stack.close();
    EXPECT_EQ(2U, layer.close.calls());
}
