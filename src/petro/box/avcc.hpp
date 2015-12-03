// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <cassert>
#include <vector>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// MPEG-4 decoder configuration
    class avcc : public box
    {

    public:

        static const std::string TYPE;

    public:
        avcc(std::weak_ptr<box> parent);

        void read(uint64_t size, byte_stream& bs);

        virtual std::string describe() const;

        std::vector<uint8_t> sequence_parameter_set(uint32_t index) const;

        std::vector<uint8_t> picture_parameter_set(uint32_t index) const;

        uint8_t length_size() const;

    private:

        /// always 1, if not, the decoder should not decode the stream.
        uint8_t m_configuration_version;

        /// profile code in ISO IEC 14496-10
        uint8_t m_avc_profile_indication;

        /// exactly the same byte that occurs between profile_IDC and level_IDC
        /// in the SPS
        uint8_t m_profile_compatibility;

        /// the level code
        uint8_t m_avc_level_indication;

        /// the length in bytes of the NALUnitLength field in an AVC video
        /// sample (subtracted by one).
        uint8_t m_length_size_minus_one;

        /// the number of SPSs that are used as the initial set of SPSs for
        /// decoding the AVC elementary stream
        uint8_t m_num_of_sequence_parameter_sets;

        /// the SPSs
        std::vector<std::vector<uint8_t>> m_sequence_parameter_sets;

        /// the number of PPS that are used as the initial set of PPSs for
        /// decoding the AVC elementary stream
        uint8_t m_num_of_picture_parameter_sets;

        /// the SPSs
        std::vector<std::vector<uint8_t>> m_picture_parameter_sets;
    };

}
}
