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
    struct dummy_trak : public petro::box::box
    {
        dummy_trak() :
           petro::box::box("trak", std::weak_ptr<box>())
        { }
    };

    struct dummy_mp4a : public petro::box::box
    {
        dummy_mp4a(std::shared_ptr<petro::box::box> parent) :
           petro::box::box("mp4a", parent)
        { }
    };

    struct dummy_root
    {
        dummy_root(std::shared_ptr<dummy_mp4a> mp4a):
            m_dummy_mp4a(mp4a)
        { }

        std::shared_ptr<const petro::box::box> get_child(
            const std::string& type) const
        {
            if (type == "mp4a")
                return m_dummy_mp4a;
            return nullptr;
        }

        std::shared_ptr<const dummy_mp4a> m_dummy_mp4a;
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

    auto trak = std::make_shared<dummy_trak>();
    auto mp4a = std::make_shared<dummy_mp4a>(trak);

    auto esds = std::make_shared<petro::box::esds>(trak);
    std::vector<uint8_t> esds_data =
    {
        0x00, // full_box version
        0x00, 0x00, 0x00, // full_box flag
        0x03, // esds elemetary_stream_descriptor_tag
        // elemetary_stream_descriptor (from test.mp4 file)
        0x19, 0x00, 0x02, 0x00, 0x04, 0x11, 0x40, 0x15,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xfd, 0x75, 0x05, 0x02, 0x11, 0x90, 0x06,
        0x01, 0x02
    };
    auto bs = petro::byte_stream(esds_data.data(), esds_data.size());
    esds->read(esds_data.size() + 8, bs);
    mp4a->add_child(esds);

    auto root = std::make_shared<dummy_root>(mp4a);

    layer.root.set_return(root);

    EXPECT_FALSE(stack.open());
    EXPECT_EQ(1U, layer.close.calls());
    EXPECT_TRUE(stack.open());

    EXPECT_EQ(2U, stack.mpeg_audio_object_type());
    EXPECT_EQ(3U, stack.frequency_index());
    EXPECT_EQ(2U, stack.channel_configuration());
    EXPECT_EQ("trak", stack.trak()->type());

    stack.close();
    EXPECT_EQ(2U, layer.close.calls());
}
