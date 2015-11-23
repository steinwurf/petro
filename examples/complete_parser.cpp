// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void print_box(petro::box::box* b, uint32_t level = 0)
{
    std::stringstream ss(b->describe());
    std::string line;
    while(std::getline(ss, line))
    {
        for (uint32_t i = 0; i < level; ++i)
        {
            std::cout << "    ";
        }
        std::cout << line << std::endl;
    }

    for(auto child : b->children())
    {
        print_box(child, level + 1);
    }
}

int main(int argc, char* argv[])
{

    if (argc != 2 || std::string(argv[1]) == "--help")
    {
        auto usage = "./complete_parser MP4_FILE";
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
        petro::box::ftyp,
        petro::box::pdin,
        petro::box::moov<petro::parser<
            petro::box::mvhd,
            petro::box::trak<petro::parser<
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
            >>,
            petro::box::mvex<petro::parser<
                petro::box::mehd,
                petro::box::trex
            >>,
            petro::box::ipmc,
            petro::box::udta<petro::parser<
                petro::box::cprt
            >>
        >>,
        petro::box::moof<petro::parser<
            petro::box::mfhd,
            petro::box::traf<petro::parser<
                petro::box::tfhd,
                petro::box::trun,
                petro::box::sdtp,
                petro::box::sbgp,
                petro::box::subs
            >>
        >>,
        petro::box::mfra<petro::parser<
            petro::box::tfra,
            petro::box::mfro
        >>,
        petro::box::mdat,
        petro::box::free,
        petro::box::skip,
        petro::box::meta<petro::parser<
            petro::box::hdlr,
            petro::box::dinf<petro::parser<
                petro::box::dref<petro::parser<
                    petro::box::url,
                    petro::box::urn
                >>
            >>,
            petro::box::ipmc,
            petro::box::iloc,
            petro::box::ipro<petro::parser<
                petro::box::sinf<petro::parser<
                    petro::box::frma,
                    petro::box::imif,
                    petro::box::schm,
                    petro::box::schi
                >>
            >>,
            petro::box::iinf,
            petro::box::xml,
            petro::box::bxml,
            petro::box::pitm
        >>
    > parser;

    auto root = new petro::box::root();
    parser.read(root, (uint8_t*)data.data(), data.size());

    for (auto box : root->children())
    {
        print_box(box);
    }

    delete root;

    return 0;
}