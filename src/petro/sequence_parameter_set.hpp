// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "bit_stream.hpp"

namespace petro
{
/// Based on https://www.itu.int/rec/T-REC-H.264-201402-S/en
class sequence_parameter_set
{
public:

    static std::shared_ptr<sequence_parameter_set> parse(
        const uint8_t* data, uint64_t size, std::error_code& error)
    {
        std::shared_ptr<sequence_parameter_set> sps(
            new sequence_parameter_set(data, size));
        sps->parse(error);
        if (error)
            return nullptr;
        else
            return sps;
    }

private:

    sequence_parameter_set(const uint8_t* data, uint64_t size) :
        m_bs(data, size)
    { }

    void parse(std::error_code& error)
    {
        m_bs.seek(0, error);
        if (error)
            return;

        uint8_t forbidden_zero_bit = 0;
        m_bs.read_bit(forbidden_zero_bit, error);
        if (error)
            return;

        if (forbidden_zero_bit != 0U)
        {
            error = petro::error::invalid_sps;
            return;
        }

        m_bs.skip(2, error); // nal_ref_idc
        if (error)
            return;

        uint8_t nal_unit_type = 0;
        m_bs.read_bits(nal_unit_type, 5, error);
        if (error)
            return;

        if (nal_unit_type != 7U)
        {
            error = petro::error::invalid_sps;
            return;
        }

        m_bs.read_bits(m_profile_idc, 8, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set0_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set1_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set2_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set3_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set4_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_constraint_set5_flag, error);
        if (error)
            return;

        // reserved_zero_2bits
        m_bs.skip(2, error);
        if (error)
            return;

        m_bs.read_bits(m_level_idc, 8, error);
        if (error)
            return;

        m_bs.read_unsigned_exponential_golomb_code(
            m_seq_parameter_set_id, error);
        if (error)
            return;


        if (m_profile_idc == 44 || m_profile_idc == 83 ||
            m_profile_idc == 86 || m_profile_idc == 100 ||
            m_profile_idc == 110 || m_profile_idc == 118 ||
            m_profile_idc == 122 || m_profile_idc == 128 ||
            m_profile_idc == 134 || m_profile_idc == 138 ||
            m_profile_idc == 139 || m_profile_idc == 244)
        {
            m_bs.read_unsigned_exponential_golomb_code(
                m_chroma_format_idc, error);
            if (error)
                return;

            if (m_chroma_format_idc == 3)
            {
                m_bs.read_bit(m_separate_colour_plane_flag, error);
                if (error)
                    return;

            }

            m_bs.read_unsigned_exponential_golomb_code(m_bit_depth_luma, error);
            if (error)
                return;

            m_bit_depth_luma += 8;
            m_bs.read_unsigned_exponential_golomb_code(
                m_bit_depth_chroma, error);
            if (error)
                return;

            m_bit_depth_chroma += 8;

            m_bs.read_bit(m_qpprime_y_zero_transform_bypass_flag, error);
            if (error)
                return;

            m_bs.read_bit(m_seq_scaling_matrix_present_flag, error);
            if (error)
                return;


            if (m_seq_scaling_matrix_present_flag)
            {
                uint8_t seq_scaling_matrix_size =
                    ((m_chroma_format_idc != 3) ? 8 : 12);
                for (uint8_t i = 0; i < seq_scaling_matrix_size; ++i)
                {
                    m_bs.read_bit(m_seq_scaling_list_present_flag, error);
                    if (error)
                        return;

                    if (m_seq_scaling_list_present_flag)
                    {
                        uint8_t size_of_scaling_list = (i < 6) ? 16 : 64;
                        int64_t last_scale = 8;
                        int64_t next_scale = 8;
                        for (uint8_t j = 0; j < size_of_scaling_list; ++j)
                        {
                            if (next_scale != 0)
                            {
                                int32_t delta_scale = 0;
                                m_bs.read_signed_exponential_golomb_code(
                                    delta_scale, error);
                                if (error)
                                    return;

                                next_scale =
                                    (last_scale + delta_scale + 256) % 256;
                            }
                            last_scale =
                                (next_scale == 0) ? last_scale : next_scale;
                        }
                    }
                }
            }
        }
        else if (m_profile_idc == 183)
        {
            m_chroma_format_idc = 0;
        }
        else
        {
            m_chroma_format_idc = 1;
        }

        m_bs.read_unsigned_exponential_golomb_code(m_log2_max_frame_num, error);
        if (error)
            return;

        m_log2_max_frame_num += 4;

        m_bs.read_unsigned_exponential_golomb_code(m_pic_order_cnt_type, error);
        if (error)
            return;

        if (m_pic_order_cnt_type == 0)
        {
            m_bs.read_unsigned_exponential_golomb_code(
                m_log2_max_pic_order_cnt_lsb, error);
            if (error)
                return;

            m_log2_max_pic_order_cnt_lsb += 4;
        }
        else if (m_pic_order_cnt_type == 1)
        {
            m_bs.read_bit(m_delta_pic_order_always_zero_flag, error);
            if (error)
                return;

            m_bs.read_bit(m_offset_for_non_ref_pic, error);
            if (error)
                return;

            m_bs.read_bit(m_offset_for_top_to_bottom_field, error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(
                m_num_ref_frames_in_pic_order_cnt_cycle, error);
            if (error)
                return;

            for (uint32_t i = 0;
                 i < m_num_ref_frames_in_pic_order_cnt_cycle; i++)
            {
                int32_t offset_for_ref_frame = 0;
                m_bs.read_signed_exponential_golomb_code(
                    offset_for_ref_frame, error);
                if (error)
                    return;

            }
        }

        m_bs.read_unsigned_exponential_golomb_code(m_num_ref_frames, error);
        if (error)
            return;

        m_bs.read_bit(m_gaps_in_frame_num_value_allowed_flag, error);
        if (error)
            return;


        m_bs.read_unsigned_exponential_golomb_code(m_pic_width_in_mbs, error);
        if (error)
            return;

        m_pic_width_in_mbs += 1;

        m_bs.read_unsigned_exponential_golomb_code(
            m_pic_height_in_map_units, error);
        if (error)
            return;

        m_pic_height_in_map_units += 1;

        m_bs.read_bit(m_frame_mbs_only_flag, error);
        if (error)
            return;

        if (!m_frame_mbs_only_flag)
        {
            m_bs.read_bit(m_mb_adaptive_frame_field_flag, error);
            if (error)
                return;

        }
        m_bs.read_bit(m_direct_8x8_inference_flag, error);
        if (error)
            return;

        m_bs.read_bit(m_frame_cropping_flag, error);
        if (error)
            return;


        if (m_frame_cropping_flag)
        {
            m_bs.read_unsigned_exponential_golomb_code(
                m_frame_crop_left_offset, error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(
                m_frame_crop_right_offset, error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(
                m_frame_crop_top_offset, error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(
                m_frame_crop_bottom_offset, error);
            if (error)
                return;

        }
        m_bs.read_bit(m_vui_parameters_present_flag, error);
        if (error)
            return;


        uint32_t pic_width_in_samples = m_pic_width_in_mbs * 16;
        m_width =
            pic_width_in_samples -
            (m_frame_crop_left_offset * 2) -
            (m_frame_crop_right_offset * 2);

        // height of each decoded picture in units of macroblocks
        uint32_t pic_height_in_mbs =
            m_pic_height_in_map_units * (2 - m_frame_mbs_only_flag);

        uint32_t pic_height_in_samples = pic_height_in_mbs * 16;
        m_height =
            pic_height_in_samples -
            (m_frame_crop_top_offset * 2) -
            (m_frame_crop_bottom_offset * 2);
    }

public:

    uint32_t width() const
    {
        return m_width;
    }

    uint32_t height() const
    {
        return m_height;
    }

    uint8_t profile_idc() const
    {
        return m_profile_idc;
    }

    uint8_t constraint_set0_flag() const
    {
        return m_constraint_set0_flag;
    }

    uint8_t constraint_set1_flag() const
    {
        return m_constraint_set1_flag;
    }

    uint8_t constraint_set2_flag() const
    {
        return m_constraint_set2_flag;
    }

    uint8_t constraint_set3_flag() const
    {
        return m_constraint_set3_flag;
    }

    uint8_t constraint_set4_flag() const
    {
        return m_constraint_set4_flag;
    }

    uint8_t constraint_set5_flag() const
    {
        return m_constraint_set5_flag;
    }

    uint8_t level_idc() const
    {
        return m_level_idc;
    }

    uint32_t seq_parameter_set_id() const
    {
        return m_seq_parameter_set_id;
    }

    uint32_t chroma_format_idc() const
    {
        return m_chroma_format_idc;
    }

    uint8_t gaps_in_frame_num_value_allowed_flag() const
    {
        return m_gaps_in_frame_num_value_allowed_flag;
    }

    uint32_t pic_width_in_mbs() const
    {
        return m_pic_width_in_mbs;
    }

    uint32_t pic_height_in_map_units() const
    {
        return m_pic_height_in_map_units;
    }

    uint32_t log2_max_frame_num() const
    {
        return m_log2_max_frame_num;
    }

    uint8_t frame_mbs_only_flag() const
    {
        return m_frame_mbs_only_flag;
    }

    uint8_t direct_8x8_inference_flag() const
    {
        return m_direct_8x8_inference_flag;
    }

    uint8_t frame_cropping_flag() const
    {
        return m_frame_cropping_flag;
    }

    uint8_t vui_parameters_present_flag() const
    {
        return m_vui_parameters_present_flag;
    }

    uint8_t separate_colour_plane_flag() const
    {
        return m_separate_colour_plane_flag;
    }

    uint32_t bit_depth_luma() const
    {
        return m_bit_depth_luma;
    }

    uint32_t bit_depth_chroma() const
    {
        return m_bit_depth_chroma;
    }

    uint8_t qpprime_y_zero_transform_bypass_flag() const
    {
        return m_qpprime_y_zero_transform_bypass_flag;
    }

    uint8_t seq_scaling_matrix_present_flag() const
    {
        return m_seq_scaling_matrix_present_flag;
    }

    uint8_t seq_scaling_list_present_flag() const
    {
        return m_seq_scaling_list_present_flag;
    }

    uint32_t log2_max_pic_order_cnt_lsb() const
    {
        return m_log2_max_pic_order_cnt_lsb;
    }

    uint8_t delta_pic_order_always_zero_flag() const
    {
        return m_delta_pic_order_always_zero_flag;
    }

    uint8_t offset_for_non_ref_pic() const
    {
        return m_offset_for_non_ref_pic;
    }

    uint8_t offset_for_top_to_bottom_field() const
    {
        return m_offset_for_top_to_bottom_field;
    }

    uint32_t num_ref_frames_in_pic_order_cnt_cycle() const
    {
        return m_num_ref_frames_in_pic_order_cnt_cycle;
    }

    uint32_t num_ref_frames() const
    {
        return m_num_ref_frames;
    }

    uint32_t frame_crop_left_offset() const
    {
        return m_frame_crop_left_offset;
    }

    uint32_t frame_crop_right_offset() const
    {
        return m_frame_crop_right_offset;
    }

    uint32_t frame_crop_top_offset() const
    {
        return m_frame_crop_top_offset;
    }

    uint32_t frame_crop_bottom_offset() const
    {
        return m_frame_crop_bottom_offset;
    }

    uint32_t pic_order_cnt_type() const
    {
        return m_pic_order_cnt_type;
    }

    uint8_t mb_adaptive_frame_field_flag() const
    {
        return m_mb_adaptive_frame_field_flag;
    }

    const uint8_t* data() const
    {
        return m_bs.data();
    }

    uint64_t size() const
    {
        return m_bs.size();
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << "sps:" << std::endl;
        ss << "      width: " << m_width << std::endl;
        ss << "      height: " << m_height << std::endl;
        return ss.str();
    }

private:

    bit_stream m_bs;

    uint32_t m_height;
    uint32_t m_width;

    uint32_t m_bit_depth_chroma;
    uint32_t m_bit_depth_luma;
    uint32_t m_log2_max_frame_num;
    uint32_t m_log2_max_pic_order_cnt_lsb;

    /// The height in slice group map units of a decoded frame or field.
    uint32_t m_pic_height_in_map_units;

    /// The width of each decoded picture in units of macroblocks.
    uint32_t m_pic_width_in_mbs;

    uint32_t m_num_ref_frames;
    uint32_t m_num_ref_frames_in_pic_order_cnt_cycle = 0;
    uint32_t m_pic_order_cnt_type;
    uint32_t m_seq_parameter_set_id;

    uint32_t m_chroma_format_idc = 0;
    uint8_t m_level_idc;
    uint8_t m_profile_idc;

    uint8_t m_offset_for_non_ref_pic;

    uint8_t m_offset_for_top_to_bottom_field;

    uint8_t m_constraint_set0_flag;
    uint8_t m_constraint_set1_flag;
    uint8_t m_constraint_set2_flag;
    uint8_t m_constraint_set3_flag;
    uint8_t m_constraint_set4_flag;
    uint8_t m_constraint_set5_flag;
    uint8_t m_delta_pic_order_always_zero_flag;
    uint8_t m_direct_8x8_inference_flag;
    uint8_t m_frame_cropping_flag;

    /// 0 specifies that coded pictures of the coded video sequence may
    /// either be coded fields or coded frames.
    /// 1 specifies that every coded picture of the coded video sequence is
    /// a coded frame containing only frame macroblocks.
    uint8_t m_frame_mbs_only_flag;

    uint8_t m_gaps_in_frame_num_value_allowed_flag;
    uint8_t m_mb_adaptive_frame_field_flag;
    uint8_t m_qpprime_y_zero_transform_bypass_flag;
    uint8_t m_separate_colour_plane_flag;
    uint8_t m_seq_scaling_list_present_flag;
    uint8_t m_seq_scaling_matrix_present_flag;
    uint8_t m_vui_parameters_present_flag;

    /// Specifies the samples of the pictures in the coded video sequence
    /// that are output from the decoding process, in terms of a rectangular
    /// region specified in frame coordinates for output.
    /// When m_frame_cropping_flag is equal to 0, the values of shall be
    /// inferred to be equal to 0.
    uint32_t m_frame_crop_left_offset = 0;
    uint32_t m_frame_crop_right_offset = 0;
    uint32_t m_frame_crop_top_offset = 0;
    uint32_t m_frame_crop_bottom_offset = 0;
};
}
