#include <petro/extractor/h264_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <gtest/gtest.h>

void compare_vectors(std::vector<char> actual, std::vector<char> expected){

    std::vector<char> output(actual.begin()+4, actual.end());
    EXPECT_EQ(output, expected);

}

TEST(test_h264_extractor, test_h264_file)
{

    auto test_filename = "test.h264";
    std::ifstream test_h264(test_filename, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());

    auto test_filename_2 = "test.mp4";
    std::ifstream test_mp4(test_filename_2, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());

    petro::extractor::h264_extractor extractor(test_mp4);



    auto test_sps = extractor.next_nalu();
    std::vector<char> sps_v(static_cast<int>(test_sps.size()));
    test_h264.read(sps_v.data(), static_cast<int>(test_sps.size()) + 4);

    compare_vectors(sps_v, test_sps);

    // auto test_pps = extractor.next_nalu();
    // std::vector<char> pps_v(static_cast<int>(test_pps.size()));
    // test_h264.read(pps_v.data(), static_cast<int>(test_pps.size()));
    //
    // compare_vectors(pps_v, test_pps);

    // while (extractor.has_next_nalu()) {
    //     auto nalu = extractor.next_nalu();
    //     uint16_t size = static_cast<uint16_t>(nalu.size());
    //     std::vector<char> temp(size);
    //     test_h264.read(temp.data(), size);
    //     compare_vectors(temp, nalu);
    //     //EXPECT_EQ(temp, nalu);
    // }

    test_h264.close();
    test_mp4.close();
}
