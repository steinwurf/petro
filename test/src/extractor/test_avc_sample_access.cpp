// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/avc_sample_access.hpp>

#include <gtest/gtest.h>

TEST(extractor_avc_sample_access, api)
{
    petro::extractor::avc_sample_access access;

    std::vector<std::vector<uint8_t>> expected_nalus
    {
        {
            0x01, 0x02, 0x03
        },
        {
            0x01, 0x02, 0x03, 0x04
        },
        {
            0x01, 0x02
        }
    };

    {
        uint32_t size_length = 4;
        SCOPED_TRACE(testing::Message() << "size length: " << size_length);
        std::vector<uint8_t> avc_sample
        {
            0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x03,
            0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
            0x00, 0x00, 0x00, 0x02, 0x01, 0x02
        };

        access.read(size_length, avc_sample.data(), avc_sample.size());
        ASSERT_EQ(3U, access.nalu_count());

        for (uint32_t i = 0; i < access.nalu_count(); ++i)
        {
            auto nalu_size = access.nalu_size_at(i);
            ASSERT_EQ(expected_nalus[i].size(), nalu_size);
            for (uint32_t j = 0; j < nalu_size; ++j)
            {
                EXPECT_EQ(expected_nalus[i][j], access.nalu_at(i)[j]);
            }
        }
    }
    {
        uint32_t size_length = 3;
        SCOPED_TRACE(testing::Message() << "size length: " << size_length);
        std::vector<uint8_t> avc_sample
        {
            0x00, 0x00, 0x03, 0x01, 0x02, 0x03,
            0x00, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
            0x00, 0x00, 0x02, 0x01, 0x02
        };

        access.read(size_length, avc_sample.data(), avc_sample.size());
        ASSERT_EQ(3U, access.nalu_count());

        for (uint32_t i = 0; i < access.nalu_count(); ++i)
        {
            auto nalu_size = access.nalu_size_at(i);
            ASSERT_EQ(expected_nalus[i].size(), nalu_size);
            for (uint32_t j = 0; j < nalu_size; ++j)
            {
                EXPECT_EQ(expected_nalus[i][j], access.nalu_at(i)[j]);
            }
        }
    }
    {
        uint32_t size_length = 2;
        SCOPED_TRACE(testing::Message() << "size length: " << size_length);
        std::vector<uint8_t> avc_sample
        {
            0x00, 0x03, 0x01, 0x02, 0x03,
            0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
            0x00, 0x02, 0x01, 0x02
        };

        access.read(size_length, avc_sample.data(), avc_sample.size());
        ASSERT_EQ(3U, access.nalu_count());

        for (uint32_t i = 0; i < access.nalu_count(); ++i)
        {
            auto nalu_size = access.nalu_size_at(i);
            ASSERT_EQ(expected_nalus[i].size(), nalu_size);
            for (uint32_t j = 0; j < nalu_size; ++j)
            {
                EXPECT_EQ(expected_nalus[i][j], access.nalu_at(i)[j]);
            }
        }
    }
    {
        uint32_t size_length = 1;
        SCOPED_TRACE(testing::Message() << "size length: " << size_length);
        std::vector<uint8_t> avc_sample
        {
            0x03, 0x01, 0x02, 0x03,
            0x04, 0x01, 0x02, 0x03, 0x04,
            0x02, 0x01, 0x02
        };

        access.read(size_length, avc_sample.data(), avc_sample.size());
        ASSERT_EQ(3U, access.nalu_count());

        for (uint32_t i = 0; i < access.nalu_count(); ++i)
        {
            auto nalu_size = access.nalu_size_at(i);
            ASSERT_EQ(expected_nalus[i].size(), nalu_size);
            for (uint32_t j = 0; j < nalu_size; ++j)
            {
                EXPECT_EQ(expected_nalus[i][j], access.nalu_at(i)[j]);
            }
        }
    }
}
