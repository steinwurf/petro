// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>
#include <petro/utils.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// void print_box(petro::box::box* b, uint32_t level = 0)
// {
//     // skip unknown boxes.
//     if (dynamic_cast<petro::box::unknown*>(b) != nullptr)
//         return;

//     std::stringstream ss(b->describe());
//     std::string line;
//     while(std::getline(ss, line))
//     {
//         for (uint32_t i = 0; i < level; ++i)
//         {
//             std::cout << "    ";
//         }
//         std::cout << line << std::endl;
//     }

//     for(auto child : b->children())
//     {
//         print_box(child, level + 1);
//     }
// }

// std::vector<uint32_t> get_sample_times(petro::box::stts* stts)
// {
//     assert(stts != nullptr);
//     std::vector<uint32_t> results;
//     for (const auto& entry : stts->entries())
//     {
//         for (uint32_t i = 0; i < entry.first; ++i)
//         {
//             results.push_back(entry.second);
//         }
//     }
//     return results;
// }

// std::vector<uint32_t> get_key_frames(petro::box::stss* stss)
// {
//     assert(stss != nullptr);
//     return stss->entries();
// }

// std::vector<uint32_t> get_sample_sizes(petro::box::stsz* stsz)
// {
//     assert(stsz != nullptr);

//     if (stsz->sample_size() != 0)
//     {
//         return std::vector<uint32_t>(stsz->sample_count(), stsz->sample_size());
//     }

//     std::vector<uint32_t> results;
//     for (const auto& entry : stsz->entries())
//     {
//         results.push_back(entry);
//     }

//     return results;
// }

// std::vector<uint32_t> get_chunk_offsets(petro::box::stco* stco)
// {
//     assert(stco != nullptr);
//     return stco->entries();
// }

// std::vector<uint32_t> get_sample_to_chunk(petro::box::stsc* stsc)
// {
//     assert(stsc != nullptr);

//     auto stsc_entries = stsc->entries();
//     std::vector<uint32_t> results;
//     for (uint32_t i = 1; i < stsc->entry_count(); ++i)
//     {
//         const auto& entry = stsc_entries[i - 1];
//         const auto& next_entry = stsc_entries[i];

//         for (uint32_t j = 0; j < entry.samples_per_chunk; ++j)
//         {

//         }
//     }

//     return results;
// }

std::vector<std::vector<uint8_t>> parse_mdat(petro::box::mdat* mdat)
{
    petro::byte_stream bs(mdat->data(), mdat->size());
    uint8_t byte1 = bs.read_uint8_t();
    uint8_t byte2 = bs.read_uint8_t();
    uint8_t byte3 = bs.read_uint8_t();

    std::vector<std::vector<uint8_t>> nalus;

    while (bs.size() != 0)
    {
        if (byte1 == 0U &&
            byte2 == 0U &&
            byte3 == 1U)
        {
            std::vector<uint8_t> nalu;
            nalu.push_back(bs.read_uint8_t());
            nalu.push_back(bs.read_uint8_t());
            nalu.push_back(bs.read_uint8_t());
            std::cout << "hep";
            while (bs.size() != 0)
            {
                if (nalu[nalu.size() - 2] == 0U &&
                    nalu[nalu.size() - 1] == 0U &&
                   (nalu[nalu.size() - 1] == 0U || nalu[nalu.size() - 1] == 1U))
                {
                    std::cout << "hey";
                    break;
                }
                nalu.push_back(bs.read_uint8_t());
            }
            nalus.push_back(nalu);
            std::cout << std::endl;
        }
        byte1 = byte2;
        byte2 = byte3;
        byte3 = bs.read_uint8_t();
    }
    return nalus;
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



    // auto nalus = parse_mdat(dynamic_cast<petro::box::mdat*>(mdat));

    // for (const auto& nalu : nalus)
    // {
    //     std::cout << (uint32_t)(nalu[0] & 0x1f)<< std::endl;
    //     uint8_t type = nalu[0];
    //     uint8_t nalu_ref_idc = (type >> 5) & 3;
    //     uint8_t nalu_type = type & 0x1f;

    //     std::cout << "nalu_ref_idc: " << (uint32_t)nalu_ref_idc << std::endl;
    //     std::cout << "nalu_type: " << (uint32_t)nalu_type << std::endl;
    // }

    // return 0;

    // auto traks = petro::find_boxes(boxes, "trak");

    // petro::box::box* video_track = nullptr;
    // petro::box::box* audio_track = nullptr;

    // for (auto trak : traks)
    // {
    //     auto hdlr = dynamic_cast<petro::box::hdlr*>(
    //         petro::find_box(trak->children(), "hdlr"));
    //     if (hdlr != nullptr)
    //     {
    //         auto handler_type = hdlr->handler_type();
    //         if(handler_type == "vide")
    //         {
    //             video_track = trak;
    //         }
    //         else if (handler_type == "soun")
    //         {
    //             audio_track = trak;
    //         }
    //     }
    // }

    // assert(video_track != nullptr);
    // assert(audio_track != nullptr);

    // auto sample_times = get_sample_times(dynamic_cast<petro::box::stts*>(
    //     petro::find_box(video_track->children(), "stts")));

    // auto key_frames = get_key_frames(dynamic_cast<petro::box::stss*>(
    //     petro::find_box(video_track->children(), "stss")));

    // auto sample_sizes = get_sample_sizes(dynamic_cast<petro::box::stsz*>(
    //     petro::find_box(video_track->children(), "stsz")));

    // auto chunk_offsets = get_chunk_offsets(dynamic_cast<petro::box::stco*>(
    //     petro::find_box(video_track->children(), "stco")));

    // auto samples_to_chunk = get_sample_to_chunk(dynamic_cast<petro::box::stsc*>(
    //     petro::find_box(video_track->children(), "stsc")));

    // std::cout << sample_times.size() << std::endl;
    // std::cout << key_frames.size() << std::endl;
    // std::cout << sample_sizes.size() << std::endl;
    // std::cout << chunk_offsets.size() << std::endl;
    // std::cout << samples_to_chunk.size() << std::endl;

    // for (auto box : boxes)
    // {
    //     delete box;
    // }

    return 0;
}