// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "bit_stream.hpp"

namespace petro
{
/// Based on https://www.itu.int/rec/T-REC-H.264-201402-S/en
class sequence_parameter_set
{
public:
    /// @return shared_ptr to a new sequence parameter set.
    /// @param data The data being parsed
    /// @param size Size of the data
    /// @param error An error to report
    static std::shared_ptr<sequence_parameter_set>
    parse(const uint8_t* data, uint64_t size, std::error_code& error)
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
    /// Constructor
    sequence_parameter_set(const uint8_t* data, uint64_t size) :
        m_bs(data, size)
    {
    }

    /// @param error Error to report
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

        m_bs.read_unsigned_exponential_golomb_code(m_seq_parameter_set_id,
                                                   error);
        if (error)
            return;

        if (m_profile_idc == 44 || m_profile_idc == 83 || m_profile_idc == 86 ||
            m_profile_idc == 100 || m_profile_idc == 110 ||
            m_profile_idc == 118 || m_profile_idc == 122 ||
            m_profile_idc == 128 || m_profile_idc == 134 ||
            m_profile_idc == 138 || m_profile_idc == 139 ||
            m_profile_idc == 244)
        {
            m_bs.read_unsigned_exponential_golomb_code(m_chroma_format_idc,
                                                       error);
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
            m_bs.read_unsigned_exponential_golomb_code(m_bit_depth_chroma,
                                                       error);
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

            for (uint32_t i = 0; i < m_num_ref_frames_in_pic_order_cnt_cycle;
                 i++)
            {
                int32_t offset_for_ref_frame = 0;
                m_bs.read_signed_exponential_golomb_code(offset_for_ref_frame,
                                                         error);
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

        m_bs.read_unsigned_exponential_golomb_code(m_pic_height_in_map_units,
                                                   error);
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
            m_bs.read_unsigned_exponential_golomb_code(m_frame_crop_left_offset,
                                                       error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(
                m_frame_crop_right_offset, error);
            if (error)
                return;

            m_bs.read_unsigned_exponential_golomb_code(m_frame_crop_top_offset,
                                                       error);
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
        m_width = pic_width_in_samples - (m_frame_crop_left_offset * 2) -
                  (m_frame_crop_right_offset * 2);

        // height of each decoded picture in units of macroblocks
        uint32_t pic_height_in_mbs =
            m_pic_height_in_map_units * (2 - m_frame_mbs_only_flag);

        uint32_t pic_height_in_samples = pic_height_in_mbs * 16;
        m_height = pic_height_in_samples - (m_frame_crop_top_offset * 2) -
                   (m_frame_crop_bottom_offset * 2);
    }

public:
    /// Width of the pictures in pixels
    uint32_t width() const
    {
        return m_width;
    }

    /// Height of the pictures in pixels
    uint32_t height() const
    {
        return m_height;
    }

    /// Flags
    uint8_t profile_idc() const
    {
        return m_profile_idc;
    }

    /// Flags
    uint8_t constraint_set0_flag() const
    {
        return m_constraint_set0_flag;
    }

    /// Flags
    uint8_t constraint_set1_flag() const
    {
        return m_constraint_set1_flag;
    }

    /// Flags
    uint8_t constraint_set2_flag() const
    {
        return m_constraint_set2_flag;
    }

    /// Flags
    uint8_t constraint_set3_flag() const
    {
        return m_constraint_set3_flag;
    }

    /// Flags
    uint8_t constraint_set4_flag() const
    {
        return m_constraint_set4_flag;
    }

    /// Flags
    uint8_t constraint_set5_flag() const
    {
        return m_constraint_set5_flag;
    }

    /// Flags
    uint8_t level_idc() const
    {
        return m_level_idc;
    }

    /// ID for the sequence parameter set
    uint32_t seq_parameter_set_id() const
    {
        return m_seq_parameter_set_id;
    }

    /// ID for the Chroma format
    uint32_t chroma_format_idc() const
    {
        return m_chroma_format_idc;
    }

    /// Flags
    uint8_t gaps_in_frame_num_value_allowed_flag() const
    {
        return m_gaps_in_frame_num_value_allowed_flag;
    }

    /// Picture width in MBs
    uint32_t pic_width_in_mbs() const
    {
        return m_pic_width_in_mbs;
    }

    /// Picture width in map units
    uint32_t pic_height_in_map_units() const
    {
        return m_pic_height_in_map_units;
    }

    /// Max number of frames in bits
    uint32_t log2_max_frame_num() const
    {
        return m_log2_max_frame_num;
    }

    /// Flags
    uint8_t frame_mbs_only_flag() const
    {
        return m_frame_mbs_only_flag;
    }

    /// Flags
    uint8_t direct_8x8_inference_flag() const
    {
        return m_direct_8x8_inference_flag;
    }

    /// Flags
    uint8_t frame_cropping_flag() const
    {
        return m_frame_cropping_flag;
    }

    /// Flags
    uint8_t vui_parameters_present_flag() const
    {
        return m_vui_parameters_present_flag;
    }

    /// Flags
    uint8_t separate_colour_plane_flag() const
    {
        return m_separate_colour_plane_flag;
    }

    /// Bit depth for luma coding
    uint32_t bit_depth_luma() const
    {
        return m_bit_depth_luma;
    }

    /// Bit depth for Chroma subsampling
    uint32_t bit_depth_chroma() const
    {
        return m_bit_depth_chroma;
    }

    /// Flags
    uint8_t qpprime_y_zero_transform_bypass_flag() const
    {
        return m_qpprime_y_zero_transform_bypass_flag;
    }

    /// Flags
    uint8_t seq_scaling_matrix_present_flag() const
    {
        return m_seq_scaling_matrix_present_flag;
    }

    /// Flags
    uint8_t seq_scaling_list_present_flag() const
    {
        return m_seq_scaling_list_present_flag;
    }

    /// Bits for max picture order
    uint32_t log2_max_pic_order_cnt_lsb() const
    {
        return m_log2_max_pic_order_cnt_lsb;
    }

    /// Flags
    uint8_t delta_pic_order_always_zero_flag() const
    {
        return m_delta_pic_order_always_zero_flag;
    }

    /// Flags
    uint8_t offset_for_non_ref_pic() const
    {
        return m_offset_for_non_ref_pic;
    }

    /// Flags
    uint8_t offset_for_top_to_bottom_field() const
    {
        return m_offset_for_top_to_bottom_field;
    }

    /// Flags
    uint32_t num_ref_frames_in_pic_order_cnt_cycle() const
    {
        return m_num_ref_frames_in_pic_order_cnt_cycle;
    }

    /// Number of reference frames
    uint32_t num_ref_frames() const
    {
        return m_num_ref_frames;
    }

    /// Left offset for frame cropping
    uint32_t frame_crop_left_offset() const
    {
        return m_frame_crop_left_offset;
    }
    /// Right offset for frame cropping
    uint32_t frame_crop_right_offset() const
    {
        return m_frame_crop_right_offset;
    }

    /// Top offset for frame cropping
    uint32_t frame_crop_top_offset() const
    {
        return m_frame_crop_top_offset;
    }

    /// Bottom offset for frame cropping
    uint32_t frame_crop_bottom_offset() const
    {
        return m_frame_crop_bottom_offset;
    }

    /// Flags
    uint32_t pic_order_cnt_type() const
    {
        return m_pic_order_cnt_type;
    }

    /// Flags
    uint8_t mb_adaptive_frame_field_flag() const
    {
        return m_mb_adaptive_frame_field_flag;
    }

    /// The picture data
    const uint8_t* data() const
    {
        return m_bs.data();
    }

    /// The size of the data
    uint64_t size() const
    {
        return m_bs.size();
    }

    /// @return A string describing the picture
    std::string describe() const
    {
        std::stringstream ss;
        ss << "sps:" << std::endl;
        ss << "      width: " << m_width << std::endl;
        ss << "      height: " << m_height << std::endl;
        return ss.str();
    }

private:
    /// Bit stream of the data
    bit_stream m_bs;

    /// Height of the picture
    uint32_t m_height;

    /// Width of the picture
    uint32_t m_width;

    /// Chroma bit depth
    uint32_t m_bit_depth_chroma;
    /// Luma bit depth
    uint32_t m_bit_depth_luma;
    /// Max frames in bits
    uint32_t m_log2_max_frame_num;
    /// Max pic order
    uint32_t m_log2_max_pic_order_cnt_lsb;

    /// The height in slice group map units of a decoded frame or field.
    uint32_t m_pic_height_in_map_units;

    /// The width of each decoded picture in units of macroblocks.
    uint32_t m_pic_width_in_mbs;

    /// number of Reference frames
    uint32_t m_num_ref_frames;
    /// number of Reference frames in picture order
    uint32_t m_num_ref_frames_in_pic_order_cnt_cycle = 0;
    /// Picture order type
    uint32_t m_pic_order_cnt_type;
    /// Sequence parameter set id
    uint32_t m_seq_parameter_set_id;

    /// Chroma format
    uint32_t m_chroma_format_idc = 0;
    /// Flag
    uint8_t m_level_idc;
    /// Flag
    uint8_t m_profile_idc;

    /// Flag
    uint8_t m_offset_for_non_ref_pic;

    /// Flag
    uint8_t m_offset_for_top_to_bottom_field;

    /// Flag
    uint8_t m_constraint_set0_flag;
    /// Flag
    uint8_t m_constraint_set1_flag;
    /// Flag
    uint8_t m_constraint_set2_flag;
    /// Flag
    uint8_t m_constraint_set3_flag;
    /// Flag
    uint8_t m_constraint_set4_flag;
    /// Flag
    uint8_t m_constraint_set5_flag;
    /// Flag
    uint8_t m_delta_pic_order_always_zero_flag;
    /// Flag
    uint8_t m_direct_8x8_inference_flag;
    /// Flag
    uint8_t m_frame_cropping_flag;

    /// 0 specifies that coded pictures of the coded video sequence may
    /// either be coded fields or coded frames.
    /// 1 specifies that every coded picture of the coded video sequence is
    /// a coded frame containing only frame macroblocks.
    uint8_t m_frame_mbs_only_flag;

    /// Flag
    uint8_t m_gaps_in_frame_num_value_allowed_flag;
    /// Flag
    uint8_t m_mb_adaptive_frame_field_flag;
    /// Flag
    uint8_t m_qpprime_y_zero_transform_bypass_flag;
    /// Flag
    uint8_t m_separate_colour_plane_flag;
    /// Flag
    uint8_t m_seq_scaling_list_present_flag;
    /// Flag
    uint8_t m_seq_scaling_matrix_present_flag;
    /// Flag
    uint8_t m_vui_parameters_present_flag;

    /// Specifies the samples of the pictures in the coded video sequence
    /// that are output from the decoding process, in terms of a rectangular
    /// region specified in frame coordinates for output.
    /// When m_frame_cropping_flag is equal to 0, the values of shall be
    /// inferred to be equal to 0.
    uint32_t m_frame_crop_left_offset = 0;
    /// Offset
    uint32_t m_frame_crop_right_offset = 0;
    /// Offset
    uint32_t m_frame_crop_top_offset = 0;
    /// Offset
    uint32_t m_frame_crop_bottom_offset = 0;
};
}
