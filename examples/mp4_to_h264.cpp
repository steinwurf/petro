// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>

uint32_t read_nalu_size(std::istream& file, uint8_t length_size)
{
    std::vector<uint8_t> data(length_size);
    file.read((char*)data.data(), data.size());

    uint32_t result = 0;
    for (uint8_t i = 0; i < length_size; ++i)
    {
        result |= data[i] << ((length_size - 1) - i) * 8;
    }

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
                            petro::box::stco,
                            petro::box::co64,
                            petro::box::stsz
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

    auto avcc = avc1->get_child<petro::box::avcc>();
    assert(avcc != nullptr);

    auto trak = avc1->get_parent("trak");
    assert(trak != nullptr);

    std::vector<uint64_t> chunk_offsets;
    auto stco = trak->get_child<petro::box::stco>();
    if (stco != nullptr)
    {
        chunk_offsets.resize(stco->entry_count());
        std::copy(
            stco->entries().begin(),
            stco->entries().end(),
            chunk_offsets.begin());
    }
    else
    {
        auto co64 = trak->get_child<petro::box::co64>();
        assert(co64 != nullptr);
        chunk_offsets.resize(co64->entry_count());
        std::copy(
            co64->entries().begin(),
            co64->entries().end(),
            chunk_offsets.begin());
    }

    auto stsc = trak->get_child<petro::box::stsc>();
    assert(stsc != nullptr);

    auto stsz = trak->get_child<petro::box::stsz>();
    assert(stsz != nullptr);

    // create output file
    std::ofstream h264_file(argv[2], std::ios::binary);

    // write sps and pps
    std::vector<char> start_code = {0, 0, 0, 1};
    auto sps = avcc->sequence_parameter_set(0);
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)sps->data(), sps->size());
    auto pps = avcc->picture_parameter_set(0);
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)pps->data(), pps->size());

    // write video data
    std::ifstream mp4_file(filename, std::ios::binary);
    uint32_t found_samples = 0;
    auto length_size = avcc->length_size();
    for (uint32_t chunk = 0; chunk < chunk_offsets.size(); ++chunk)
    {
        mp4_file.seekg(chunk_offsets[chunk]);
        for (uint32_t sample = 0; sample < stsc->samples_for_chunk(chunk);
             ++sample)
        {
            // The sample size describes the size of the Access Unit (AU) of the
            // h264 data. This means multiple NALUs can be in the same "sample".
            // Each of these NALUs needs a preceeding Annex B start code.
            uint32_t sample_size = stsz->sample_size(found_samples);
            while (sample_size > 0)
            {
                auto nalu_size = read_nalu_size(mp4_file, length_size);
                sample_size -= length_size;
                h264_file.write(start_code.data(), start_code.size());
                std::vector<char> temp(nalu_size);
                mp4_file.read(temp.data(), nalu_size);
                sample_size -= nalu_size;
                h264_file.write(temp.data(), nalu_size);
            }
            found_samples++;
        }
    }

    mp4_file.close();
    h264_file.close();
    return 0;
}
