// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>
#include <petro/utils.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace
{
    struct sample
    {
        uint32_t size;
        uint32_t delta;
        bool random_access;
    };

    struct chunk
    {
        uint32_t sample_count;
        std::vector<sample> samples;
        uint32_t offset;
    };
}

int main(int argc, char* argv[])
{
    if (argc != 2 || std::string(argv[1]) == "--help")
    {
        auto usage = "./access_unit_parser MP4_FILE";
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

    using trak_type = petro::box::trak<
        petro::parser<
            petro::box::tkhd,
            petro::box::tref,
            petro::box::edts<petro::parser<
                petro::box::elst
            >>,
            petro::box::mdia<petro::parser<
                petro::box::mdhd,
                petro::box::hdlr,
                petro::box::minf<petro::parser<
                    petro::box::vmhd,
                    petro::box::smhd,
                    petro::box::hmhd,
                    petro::box::nmhd,
                    petro::box::dinf<petro::parser<
                        petro::box::dref<petro::parser<
                            petro::box::url,
                            petro::box::urn
                        >>
                    >>,
                    petro::box::stbl<petro::parser<
                        petro::box::stsd,
                        petro::box::stts,
                        petro::box::ctts,
                        petro::box::stsc,
                        petro::box::stsz,
                        petro::box::stz2,
                        petro::box::stco,
                        petro::box::co64,
                        petro::box::stss,
                        petro::box::stsh,
                        petro::box::padb,
                        petro::box::stdp,
                        petro::box::sdtp,
                        petro::box::sbgp,
                        petro::box::sgpd,
                        petro::box::subs
                    >>
                >>,
                petro::box::mdhd
            >>
        >>;

    petro::parser<
        petro::box::moov<petro::parser<
            petro::box::mvhd,
            trak_type
        >>,
        petro::box::mdat
    > parser;

    auto root = new petro::box::root();
    parser.read(root, (uint8_t*)data.data(), data.size());

    auto avc1 = root->get_child("avc1");
    assert(avc1 != nullptr);

    auto trak = avc1->get_parent("trak");
    assert(trak != nullptr);

    auto mvex = root->get_child("mvex");
    // don't handle special case with fragmented samples
    assert(mvex == nullptr);

    auto stco = dynamic_cast<const petro::box::stco*>(trak->get_child("stco"));
    assert(stco != nullptr);

    auto stsz = dynamic_cast<const petro::box::stsz*>(trak->get_child("stsz"));
    assert(stsz != nullptr);

    auto stsc = dynamic_cast<const petro::box::stsc*>(trak->get_child("stsc"));
    assert(stsc != nullptr);

    auto chunk_offsets = stco->entries();

    auto samples_to_chunks = stsc->entries();

    std::vector<chunk> chunks;
    auto sample = 0;
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        chunk c;
        c.sample_count += stsc->samples_for_chunk(i);
        // for (int i = 0; i < c.sample_count; ++i)
        // {
        //     sample s;
        //     s.
        //     c.samples.push_back(s);
        // }
    }

    std::cout << sample << std::endl;

    delete root;

    return 0;
}