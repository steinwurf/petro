// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/sequence_parameter_set.hpp>

#include <vector>

#include <gtest/gtest.h>

TEST(test_sequence_parameter_set, check_variables)
{
    std::vector<uint8_t> data =
        {
            0x67, 0x42, 0x00, 0x0A, 0xF8, 0x41, 0xA2
        };

    petro::sequence_parameter_set sps(data.data(), data.size());
    EXPECT_EQ(66U, sps.profile_idc());
    EXPECT_EQ(0U, sps.constraint_set0_flag());
    EXPECT_EQ(0U, sps.constraint_set1_flag());
    EXPECT_EQ(0U, sps.constraint_set2_flag());
    EXPECT_EQ(0U, sps.constraint_set3_flag());
    EXPECT_EQ(10U, sps.level_idc());
    EXPECT_EQ(0U, sps.seq_parameter_set_id());
    EXPECT_EQ(4U, sps.log2_max_frame_num());
    EXPECT_EQ(0U, sps.pic_order_cnt_type());
    EXPECT_EQ(4U, sps.log2_max_pic_order_cnt_lsb());
    EXPECT_EQ(0U, sps.num_ref_frames_in_pic_order_cnt_cycle());
    EXPECT_EQ(0U, sps.gaps_in_frame_num_value_allowed_flag());
    EXPECT_EQ(8U, sps.pic_width_in_mbs());
    EXPECT_EQ(6U, sps.pic_height_in_map_units());
    EXPECT_EQ(1U, sps.frame_mbs_only_flag());
    EXPECT_EQ(0U, sps.direct_8x8_inference_flag());
    EXPECT_EQ(0U, sps.frame_cropping_flag());
    EXPECT_EQ(0U, sps.vui_parameters_present_flag());
    EXPECT_EQ(128U, sps.width());
    EXPECT_EQ(96U, sps.height());
}

TEST(test_sequence_parameter_set, width512_height288)
{
    std::vector<uint8_t> data =
        {
            0x67, 0x64, 0x00, 0x15, 0xac, 0xc8, 0x60, 0x20,
            0x09, 0x6c, 0x04, 0x40, 0x00, 0x00, 0x03, 0x00,
            0x40, 0x00, 0x00, 0x07, 0xa3, 0xc5, 0x8b, 0x67,
            0x80
        };
    petro::sequence_parameter_set sps(data.data(), data.size());
    EXPECT_EQ(100U, sps.profile_idc());
    EXPECT_EQ(0U, sps.constraint_set0_flag());
    EXPECT_EQ(0U, sps.constraint_set1_flag());
    EXPECT_EQ(0U, sps.constraint_set2_flag());
    EXPECT_EQ(0U, sps.constraint_set3_flag());
    EXPECT_EQ(21U, sps.level_idc());
    EXPECT_EQ(0U, sps.seq_parameter_set_id());
    EXPECT_EQ(1U, sps.chroma_format_idc());
    EXPECT_EQ(0U, sps.gaps_in_frame_num_value_allowed_flag());
    EXPECT_EQ(32U, sps.pic_width_in_mbs());
    EXPECT_EQ(18U, sps.pic_height_in_map_units());
    EXPECT_EQ(1U, sps.frame_mbs_only_flag());
    EXPECT_EQ(1U, sps.direct_8x8_inference_flag());
    EXPECT_EQ(0U, sps.frame_cropping_flag());
    EXPECT_EQ(1U, sps.vui_parameters_present_flag());
    EXPECT_EQ(512U, sps.width());
    EXPECT_EQ(288U, sps.height());
}

TEST(test_sequence_parameter_set, width1280_height800)
{
    std::vector<uint8_t> data =
        {
            0x67, 0x42, 0x00, 0x20, 0xE2, 0x90, 0x0A, 0x00,
            0xCB, 0x60, 0x2D, 0xC0, 0x40, 0x40, 0x69, 0x07,
            0x89, 0x11, 0x50
        };

    petro::sequence_parameter_set sps(data.data(), data.size());
    EXPECT_EQ(1280U, sps.width());
    EXPECT_EQ(800U, sps.height());

}

TEST(test_sequence_parameter_set, width1920_height1080)
{
    std::vector<uint8_t> data =
        {
            0x67, 0x64, 0x00, 0x2A, 0xAD, 0x84, 0x01, 0x0C,
            0x20, 0x08, 0x61, 0x00, 0x43, 0x08, 0x02, 0x18,
            0x40, 0x10, 0xC2, 0x00, 0x84, 0x2B, 0x50, 0x3C,
            0x01, 0x13, 0xF2, 0xC2, 0x00, 0x00, 0x03, 0x00,
            0x02, 0x00, 0x00, 0x03, 0x00, 0x79, 0x08
        };

    petro::sequence_parameter_set sps(data.data(), data.size());
    EXPECT_EQ(1920U, sps.width());
    EXPECT_EQ(1080U, sps.height());
}

TEST(test_sequence_parameter_set, width3840_height2160)
{
    std::vector<uint8_t> data =
        {
            0x67, 0x64, 0x00, 0x33, 0xAC, 0xD1, 0x00, 0x3C,
            0x00, 0x43, 0xEC, 0x05, 0x48, 0x08, 0x08, 0x0A,
            0x00, 0x00, 0x07, 0xD2, 0x00, 0x01, 0xD4, 0xC1,
            0xC8, 0x00, 0x00, 0x0B, 0xEB, 0xC2, 0x00, 0x00,
            0x05, 0xF5, 0xBD, 0xD4, 0x98, 0x60, 0x1F, 0x18,
            0x31, 0x12
        };

    petro::sequence_parameter_set sps(data.data(), data.size());
    EXPECT_EQ(3840U, sps.width());
    EXPECT_EQ(2160U, sps.height());
}
