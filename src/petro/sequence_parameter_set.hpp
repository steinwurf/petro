// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <sstream>
#include <vector>



#include <iostream>



#include "bit_reader.hpp"

namespace petro
{
    class sequence_parameter_set
    {

    public:
        sequence_parameter_set(std::vector<uint8_t> data):
            m_data(data)
        {
            auto bits = bit_reader(m_data);
            uint8_t forbidden_zero_bit = bits.read_1_bit();
            assert(forbidden_zero_bit == 0);
            bits.skip(2); // nal_ref_idc
            uint8_t nal_unit_type = bits.read_5_bits();
            assert(nal_unit_type == 7U); // should be 7 for SPS

            m_profile_idc = bits.read_8_bits();
            m_constraint_set0_flag = bits.read_1_bit();
            m_constraint_set1_flag = bits.read_1_bit();
            m_constraint_set2_flag = bits.read_1_bit();
            m_constraint_set3_flag = bits.read_1_bit();
            m_constraint_set4_flag = bits.read_1_bit();
            m_constraint_set5_flag = bits.read_1_bit();
            uint8_t reserved_zero_2bits  = bits.read_2_bits();
            assert(reserved_zero_2bits == 0);

            m_level_idc = bits.read_8_bits();
            m_seq_parameter_set_id = bits.read_exponential_golomb_code();

            if (m_profile_idc == 100 || m_profile_idc == 110 ||
                m_profile_idc == 118 || m_profile_idc == 122 ||
                m_profile_idc == 244 || m_profile_idc == 44 ||
                m_profile_idc == 83  || m_profile_idc == 86)
            {
                m_chroma_format_idc = bits.read_exponential_golomb_code();
                if (m_chroma_format_idc == 3)
                {
                    m_separate_colour_plane_flag = bits.read_1_bit();
                }

                m_bit_depth_luma_minus8 = bits.read_exponential_golomb_code();
                m_bit_depth_chroma_minus8 = bits.read_exponential_golomb_code();
                m_qpprime_y_zero_transform_bypass_flag = bits.read_1_bit();
                m_seq_scaling_matrix_present_flag = bits.read_1_bit();

                if (m_seq_scaling_matrix_present_flag)
                {
                    uint8_t seq_scaling_matrix_size =
                        ((m_chroma_format_idc != 3) ? 8 : 12);
                    for (uint8_t i = 0; i < seq_scaling_matrix_size; ++i)
                    {
                        m_seq_scaling_list_present_flag = bits.read_1_bit();
                        if (m_seq_scaling_list_present_flag)
                        {
                            uint8_t size_of_scaling_list = (i < 6) ? 16 : 64;
                            int64_t last_scale = 8;
                            int64_t next_scale = 8;
                            for (uint8_t j = 0; j < size_of_scaling_list; ++j)
                            {
                                if (next_scale != 0)
                                {
                                    auto delta_scale = bits.read_se();
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

            m_log2_max_frame_num_minus4 = bits.read_exponential_golomb_code();
            m_pic_order_cnt_type = bits.read_exponential_golomb_code();
            if (m_pic_order_cnt_type == 0)
            {
                m_log2_max_pic_order_cnt_lsb_minus4 =
                    bits.read_exponential_golomb_code();
            }
            else if (m_pic_order_cnt_type == 1)
            {
                m_delta_pic_order_always_zero_flag = bits.read_1_bit();
                m_offset_for_non_ref_pic = bits.read_1_bit();
                m_offset_for_top_to_bottom_field = bits.read_1_bit();
                m_num_ref_frames_in_pic_order_cnt_cycle =
                    bits.read_exponential_golomb_code();
                for (uint32_t i = 0;
                     i < m_num_ref_frames_in_pic_order_cnt_cycle; i++)
                {
                    bits.read_se(); // offset_for_ref_frame[i]
                }
            }

            m_num_ref_frames = bits.read_exponential_golomb_code();
            m_gaps_in_frame_num_value_allowed_flag = bits.read_1_bit();

            m_pic_width_in_mbs = bits.read_exponential_golomb_code() + 1;
            m_pic_height_in_map_units = bits.read_exponential_golomb_code() + 1;

            m_frame_mbs_only_flag = bits.read_1_bit();
            if (!m_frame_mbs_only_flag)
            {
                m_mb_adaptive_frame_field_flag = bits.read_1_bit();
            }
            m_direct_8x8_inference_flag = bits.read_1_bit();
            m_frame_cropping_flag = bits.read_1_bit();

            if (m_frame_cropping_flag)
            {
                m_frame_crop_left_offset = bits.read_exponential_golomb_code();
                m_frame_crop_right_offset = bits.read_exponential_golomb_code();
                m_frame_crop_top_offset = bits.read_exponential_golomb_code();
                m_frame_crop_bottom_offset =
                    bits.read_exponential_golomb_code();
            }
            m_vui_parameters_present_flag = bits.read_1_bit();

            m_width =
                ((m_pic_width_in_mbs) * 16) -
                m_frame_crop_left_offset * 2 - m_frame_crop_right_offset * 2;
            m_height =
                ((m_pic_height_in_map_units) * 16 * (2 - m_frame_mbs_only_flag)) - (m_frame_crop_top_offset * 2) - (m_frame_crop_bottom_offset * 2);
        }

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

        uint8_t chroma_format_idc() const
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

        uint32_t log2_max_frame_num_minus4() const
        {
            return m_log2_max_frame_num_minus4;
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

        uint32_t bit_depth_luma_minus8() const
        {
            return m_bit_depth_luma_minus8;
        }

        uint32_t bit_depth_chroma_minus8() const
        {
            return m_bit_depth_chroma_minus8;
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

        uint32_t log2_max_pic_order_cnt_lsb_minus4() const
        {
            return m_log2_max_pic_order_cnt_lsb_minus4;
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
            return m_data.data();
        }

        uint32_t size() const
        {
            return m_data.size();
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

        std::vector<uint8_t> m_data;

        uint32_t m_height;
        uint32_t m_width;

        uint32_t m_bit_depth_chroma_minus8;
        uint32_t m_bit_depth_luma_minus8;
        uint32_t m_log2_max_frame_num_minus4;
        uint32_t m_log2_max_pic_order_cnt_lsb_minus4;
        uint32_t m_pic_height_in_map_units;
        uint32_t m_pic_width_in_mbs;
        uint32_t m_num_ref_frames;
        uint32_t m_num_ref_frames_in_pic_order_cnt_cycle;
        uint32_t m_pic_order_cnt_type;
        uint32_t m_seq_parameter_set_id;

        uint8_t m_chroma_format_idc;
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
        uint8_t m_frame_mbs_only_flag;
        uint8_t m_gaps_in_frame_num_value_allowed_flag;
        uint8_t m_mb_adaptive_frame_field_flag;
        uint8_t m_qpprime_y_zero_transform_bypass_flag;
        uint8_t m_separate_colour_plane_flag;
        uint8_t m_seq_scaling_list_present_flag;
        uint8_t m_seq_scaling_matrix_present_flag;
        uint8_t m_vui_parameters_present_flag;

        uint32_t m_frame_crop_left_offset = 0;
        uint32_t m_frame_crop_right_offset = 0;
        uint32_t m_frame_crop_top_offset = 0;
        uint32_t m_frame_crop_bottom_offset = 0;
    };
}
