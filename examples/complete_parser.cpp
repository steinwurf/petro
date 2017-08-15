// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <boost/iostreams/device/mapped_file.hpp>


void print_box(std::shared_ptr<petro::box::box> b, uint32_t level = 0)
{
    assert(b != nullptr);
    std::stringstream ss(b->describe());
    std::string line;
    while (std::getline(ss, line))
    {
        for (uint32_t i = 0; i < level; ++i)
        {
            std::cout << "    ";
        }
        std::cout << line << std::endl;
    }

    for (auto child : b->children())
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
    auto filename = std::string(argv[1]);
    boost::iostreams::mapped_file_source mp4_file(filename);

    if (!mp4_file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 1;
    }

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

    std::error_code error;
    auto root = std::make_shared<petro::box::root>();
    parser.parse((uint8_t*)mp4_file.data(), mp4_file.size(), root, error);

    if (error)
    {
        std::cerr << error.message() << std::endl;
        return 1;
    }

    for (auto box : root->children())
    {
        print_box(box);
    }

    return 0;
}
