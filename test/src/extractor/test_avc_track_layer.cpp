// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/avc_track_layer.hpp>

#include <petro/box/data_box.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_parameter_set
{
    dummy_parameter_set(const uint8_t* data, uint32_t size) :
        m_data(data), m_size(size)
    {
    }

    const uint8_t* data() const
    {
        return m_data;
    }

    uint32_t size() const
    {
        return m_size;
    }

    const uint8_t* m_data;
    uint32_t m_size;
};

struct dummy_avcc
{
    dummy_avcc() :
        m_picture_parameter_set((const uint8_t*)1, 1U),
        m_sequence_parameter_set((const uint8_t*)2, 2U)
    {
    }

    const dummy_parameter_set* picture_parameter_set(uint32_t index) const
    {
        (void)index;
        return &m_picture_parameter_set;
    }

    const dummy_parameter_set* sequence_parameter_set(uint32_t index) const
    {
        (void)index;
        return &m_sequence_parameter_set;
    }

    uint32_t length_size() const
    {
        return 42;
    }

    dummy_parameter_set m_picture_parameter_set;
    dummy_parameter_set m_sequence_parameter_set;
};

struct dummy_avc1
{
    template <class Box>
    const std::shared_ptr<dummy_avcc> get_child() const
    {
        return std::make_shared<dummy_avcc>();
    }
};

struct dummy_trak
{
    dummy_trak() : m_dummy_avc1(std::make_shared<dummy_avc1>())
    {
    }
    const std::shared_ptr<dummy_avc1> get_child(const std::string& type) const
    {
        (void)type;
        return m_dummy_avc1;
    }

    std::shared_ptr<dummy_avc1> m_dummy_avc1;
};

struct dummy_layer
{
    stub::function<void(std::error_code)> open;
    stub::function<void()> close;
    stub::function<std::shared_ptr<dummy_trak>()> trak;
};

using dummy_stack = petro::extractor::avc_track_layer<dummy_layer, dummy_avcc>;
}

TEST(extractor_test_avc_track_layer, api)
{
    dummy_stack stack;
    dummy_layer& layer = stack;

    auto trak = std::make_shared<dummy_trak>();
    layer.trak.set_return(trak);

    std::error_code error;
    stack.open(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ((const uint8_t*)1, stack.pps_data());
    EXPECT_EQ(1U, stack.pps_size());
    EXPECT_EQ((const uint8_t*)2, stack.sps_data());
    EXPECT_EQ(2U, stack.sps_size());
    EXPECT_EQ(42U, stack.nalu_length_size());
    EXPECT_NE(nullptr, stack.trak());

    stack.close();
    EXPECT_EQ(1U, layer.close.calls());
}
