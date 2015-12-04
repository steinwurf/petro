// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>

uint32_t read_sample_size(std::istream& file)
{
    std::vector<uint8_t> data(4);
    file.read((char*)data.data(), data.size());

    uint32_t result =
       (uint32_t) data[0] << 24 |
       (uint32_t) data[1] << 16 |
       (uint32_t) data[2] << 8  |
       (uint32_t) data[3];
    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_acc MP4_INPUT ACC_OUTPUT";
        std::cout << usage << std::endl;
        return 0;
    }

    auto filename = std::string(argv[1]);

    std::ifstream check(filename, std::ios::binary);

    if (!check.is_open() || !check.good())
    {
        std::cerr << "Error reading file: " << filename << std::endl;
        return 1;
    }

    petro::parser<
        petro::box::moov<petro::parser<
            petro::box::trak<petro::parser<
                petro::box::mdia<petro::parser<
                    petro::box::hdlr,
                    petro::box::minf<petro::parser<
                        petro::box::stbl<petro::parser<
                            petro::box::stsd
                        >>
                    >>
                >>
            >>
        >>
    > parser;

    auto root = std::make_shared<petro::box::root>();
    petro::byte_stream bs(filename);

    parser.read(root, bs);

    // get needed boxes
    auto mp4a = root->get_child("mp4a");
    assert(mp4a != nullptr);
    std::cout << mp4a->describe() << std::endl;
    for (auto c : mp4a->children())
    {
        std::cout << c->describe() << std::endl;
    }

    return 0;
}
