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
        auto usage = "./mp4_to_h264 MP4_INPUT H264_OUTPUT";
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
                            petro::box::stsd,
                            petro::box::stsc,
                            petro::box::stco
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
    auto avc1 = root->get_child("avc1");
    assert(avc1 != nullptr);

    auto avcc = avc1->get_child<petro::box::avcc>("avcC");
    assert(avcc != nullptr);

    auto trak = avc1->get_parent("trak");
    assert(trak != nullptr);

    auto stco = trak->get_child<petro::box::stco>("stco");
    assert(stco != nullptr);

    auto stsc = trak->get_child<petro::box::stsc>("stsc");
    assert(stsc != nullptr);

    // create output file
    std::ofstream h264_file(argv[2], std::ios::binary);

    // write sps and pps
    std::vector<char> nalu_seperator = {0, 0, 0, 1};
    auto sps = avcc->sequence_parameter_set(0);
    h264_file.write((char*)nalu_seperator.data(), nalu_seperator.size());
    h264_file.write((char*)sps.data(), sps.size());
    auto pps = avcc->picture_parameter_set(0);
    h264_file.write((char*)nalu_seperator.data(), nalu_seperator.size());
    h264_file.write((char*)pps.data(), pps.size());

    // write video data
    std::ifstream mp4_file(filename, std::ios::binary);
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        mp4_file.seekg(stco->chunk_offset(i));
        for (uint32_t j = 0; j < stsc->samples_for_chunk(i); ++j)
        {
            h264_file.write(nalu_seperator.data(), nalu_seperator.size());

            auto sample_size = read_sample_size(mp4_file);

            std::vector<char> temp(sample_size);
            mp4_file.read(temp.data(), sample_size);
            h264_file.write(temp.data(), sample_size);
        }
    }

    h264_file.close();
    return 0;
}
