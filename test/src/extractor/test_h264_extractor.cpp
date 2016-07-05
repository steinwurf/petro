#include <petro/extractor/h264_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <gtest/gtest.h>

TEST(test_h264_extractor, test_h264_file)
{

    auto test_filename = "test.h264";
    std::ifstream test_h264(test_filename, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());

    auto test_filename_2 = "test.mp4";
    std::ifstream test_mp4(test_filename_2, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());

    petro::extractor::h264_extractor extractor(test_mp4);

    while (extractor.has_next_nalu()) {
        auto nalu = extractor.next_nalu();
        uint16_t size = static_cast<uint16_t>(nalu.size());
        std::vector<char> temp(size);
        test_h264.read(temp.data(), size);
        EXPECT_EQ(temp, nalu);
    }
}
