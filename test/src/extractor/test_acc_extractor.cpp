#include <petro/extractor/acc_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

TEST(test_acc_extractor, test_acc_file)
{
    auto test_filename = "test_acc";
    std::ifstream test_acc(test_filename, std::ios::binary);
    EXPECT_TRUE(test_acc.is_open());

    auto test_filename_2 = "test.mp4";
    std::ifstream test_mp4(test_filename_2, std::ios:binary);
    EXPECT_TRUE(test_mp4.is_open());

    petro::extractor::acc_extractor acc_extractor(test_mp4);
    EXPECT_TRUE(acc_extractor != nullptr);

    while (extractor.has_next_adts()) {
        auto adts = acc_extractor.next_adts();

        std::vector<char> temp(adts.size());
        test_acc.read(temp, adts.size());

        EXPECT_EQ(temp, adts);
    }

}
