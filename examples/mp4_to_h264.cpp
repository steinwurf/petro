// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

namespace
{
    struct chunk
    {
        uint32_t sample_count;
        std::vector<uint32_t> sample_sizes;
        uint32_t offset;
    };
}

uint32_t read_uint32_t(const uint8_t* data)
{
    uint32_t result =
           (uint32_t) data[0] << 24 |
           (uint32_t) data[1] << 16 |
           (uint32_t) data[2] << 8 |
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

    std::ifstream mp4_file(argv[1], std::ios::binary);

    if (not mp4_file.is_open() || not mp4_file.good())
    {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    std::vector<char> data((
        std::istreambuf_iterator<char>(mp4_file)),
        std::istreambuf_iterator<char>());

    petro::parser<
        petro::box::moov<petro::parser<
            petro::box::trak<petro::parser<
                petro::box::mdia<petro::parser<
                    petro::box::hdlr,
                    petro::box::minf<petro::parser<
                        petro::box::stbl<petro::parser<
                            petro::box::stsd,
                            petro::box::stsc,
                            petro::box::stsz,
                            petro::box::stco
                        >>
                    >>
                >>
            >>
        >>
    > parser;

    auto root = std::make_shared<petro::box::root>();
    parser.read(root, (uint8_t*)data.data(), data.size());

    // don't handle special case with fragmented samples
    assert(root->get_child("mvex") == nullptr);

    auto avc1 = root->get_child("avc1");
    assert(avc1 != nullptr);

    auto avcc = std::dynamic_pointer_cast<const petro::box::avcc>(avc1->get_child("avcC"));
    assert(avcc != nullptr);

    auto trak = avc1->get_parent("trak");
    assert(trak != nullptr);

    auto stco = std::dynamic_pointer_cast<const petro::box::stco>(trak->get_child("stco"));
    assert(stco != nullptr);

    auto stsz = std::dynamic_pointer_cast<const petro::box::stsz>(trak->get_child("stsz"));
    assert(stsz != nullptr);

    auto stsc = std::dynamic_pointer_cast<const petro::box::stsc>(trak->get_child("stsc"));
    assert(stsc != nullptr);

    std::ofstream h264_file(argv[2], std::ios::binary);
    // write sps and pps
    std::vector<uint8_t> nalu_seperator = {0, 0, 0, 1};
    auto sps = avcc->sequence_parameter_set(0);
    h264_file.write((char*)nalu_seperator.data(), nalu_seperator.size());
    h264_file.write((char*)sps.data(), sps.size());
    auto pps = avcc->picture_parameter_set(0);
    h264_file.write((char*)nalu_seperator.data(), nalu_seperator.size());
    h264_file.write((char*)pps.data(), pps.size());

    auto found_samples = 0;
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        auto offset = stco->chunk_offset(i);
        for (uint32_t j = 0; j < stsc->samples_for_chunk(i); ++j)
        {
            auto actual_size = read_uint32_t((uint8_t*)(data.data() + offset));
            h264_file.write((char*)nalu_seperator.data(), nalu_seperator.size());
            h264_file.write((char*)data.data() + offset + 4, actual_size);
            offset += stsz->sample_size(found_samples);
            found_samples += 1;
        }
    }

    h264_file.close();
    return 0;
}
