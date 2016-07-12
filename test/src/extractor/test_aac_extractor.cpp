#include <petro/extractor/aac_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <gtest/gtest.h>

TEST(test_aac_extractor, test_aac_file)
{
    // Open file with sanitised test data
    // File generate wit ffmpeg: ffmpeg -i test.mp4 -acodec copy test.aac
    auto test_filename = "test.aac";
    std::ifstream test_aac(test_filename, std::ios::binary);
    EXPECT_TRUE(test_aac.is_open());

    // Open file with unsanitised test data
    auto test_filename_2 = "test.mp4";
    std::ifstream test_mp4(test_filename_2, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());

    petro::extractor::aac_extractor aac_extractor(test_mp4);

    // Check that each sample is correct
    while(aac_extractor.has_next_sample())
    {
        auto sample = aac_extractor.next_sample();
        std::vector<char> temp(sample.size());
        test_aac.read(temp.data(), temp.size());
        EXPECT_EQ(sample, temp);
    }

    // Check if we are at end of file of test_aac
    //EXPECT_EQ(test_aac.peek(), std::ifstream::traits_type::eof());

    test_aac.close();
    test_mp4.close();
}
