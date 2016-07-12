#include <petro/extractor/h264_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <gtest/gtest.h>

void compare_vectors(std::vector<char> actual, std::vector<char> expected)
{
    std::vector<char> output(actual.begin() + 4, actual.end());
    EXPECT_EQ(expected, output);
}

TEST(test_h264_extractor, test_h264_file)
{
    auto test_filename = "test.h264";
    std::ifstream test_h264(test_filename, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());
    EXPECT_TRUE(test_h264.good());

    // Find last bye of test_h264 file enablig us to later
    // check if we have rad the correct amount of bytes
    auto last_byte = test_h264.seekg(0, std::ios::end).tellg();
    test_h264.seekg(0);

    auto test_filename_2 = "test.mp4";
    std::ifstream test_mp4(test_filename_2, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());
    EXPECT_TRUE(test_mp4.good());

    petro::extractor::h264_extractor extractor(test_mp4);

    // Get the SPS data from the extractor
    auto sps_begin = (char*)extractor.sps()->data();
    auto sps_end = (char*)extractor.sps()->data() + extractor.sps()->size();
    std::vector<char> test_sps(sps_begin, sps_end);

    // Because we know that we have the start code 00 00 00 01
    // at the begining of all NALU's in the test data we need
    // to read 4 extract byte
    std::vector<char> sps_v(test_sps.size() + 4);
    test_h264.read(sps_v.data(), sps_v.size());
    compare_vectors(sps_v, test_sps);

    // Get the SPS data from the extractor
    auto pps_begin = (char*)extractor.pps()->data();
    auto pps_end = (char*)extractor.pps()->data() + extractor.pps()->size();
    std::vector<char> test_pps(pps_begin, pps_end);

    std::vector<char> pps_v(test_pps.size() + 4);
    test_h264.read(pps_v.data(), pps_v.size());

    compare_vectors(pps_v, test_pps);

    while (extractor.has_next_nalu())
    {
        auto nalu = extractor.next_nalu();
        std::vector<char> temp(nalu.size() + 4);
        test_h264.read(temp.data(), temp.size());
        compare_vectors(temp, nalu);
    }

    // Check if we have read the correct amount of bytes
    EXPECT_EQ(last_byte, test_h264.tellg());

    test_h264.close();
    test_mp4.close();
}
