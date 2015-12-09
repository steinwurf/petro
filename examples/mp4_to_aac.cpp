// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <map>

/// How to generate the AAC ADTS elementary stream with Android MediaCodec
// http://stackoverflow.com/questions/18862715/how-to-generate-the-aac-adts-elementary-stream-with-android-mediacodec

/// Decoding AAC using MediaCodec API on Android
// http://stackoverflow.com/questions/12942201/decoding-aac-using-mediacodec-api-on-android

/// How to initialize MediaFormat to configure a MediaCodec to decode raw AAC data?
// http://stackoverflow.com/questions/18784781/how-to-initialize-mediaformat-to-configure-a-mediacodec-to-decode-raw-aac-data


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

void print_box(std::shared_ptr<petro::box::box> b, uint32_t level = 0)
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
// http://wiki.multimedia.cx/index.php?title=ADTS
// writes 7 - 9 bytes
// uint32_t write_adts(
//     uint8_t* data,
//     uint8_t mpeg_version,
//     uint8_t protection,
//     uint8_t mpeg_audio_object_type,
//     uint8_t sampling_frequency_index,
//     uint8_t mpeg_channel_configuration
//     uint16_t frame_length,
//     uint8_t number_of_aac_frames,
//     uint16_t crc)
// {
//     uint32_t written = 0;

//     data[0] = 0xFF;

//     assert(protection == 0 || protection == 1);
//     data[0] = 0xF0 + protection;


    // FrameLength = (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    //(RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame per ADTS frame
// }
namespace
{
uint8_t get_sampling_frequency_index(
    std::shared_ptr<const petro::box::stsd::audio_sample_entry> mp4a)
{
    std::map<uint32_t, uint8_t> frequencies {
        { 96000, 0 },
        { 88200, 1 },
        { 64000, 2 },
        { 48000, 3 },
        { 44100, 4 },
        { 32000, 5 },
        { 24000, 6 },
        { 22050, 7 },
        { 16000, 8 },
        { 12000, 9 },
        { 11025, 10 },
        { 8000, 11 },
        { 7350, 12 }
    };

    auto sample_rate = mp4a->sample_rate();

    auto sampling_frequency_index = frequencies.find(sample_rate);
    if (sampling_frequency_index != frequencies.end())
    {
        return sampling_frequency_index->second;
    }
    return 15; // frequency is written explictly


}

uint8_t get_mpeg_audio_object_type(std::shared_ptr<const petro::box::esds> esds)
{
    auto es_descriptor = esds->descriptor();
    auto decoder_config_descriptor = es_descriptor->decoder_config_descriptor();

    // verify that track is an MPEG-4 audio track
    if (decoder_config_descriptor->object_type_id() != 0x40)
        return 0;

    auto decoder_specific_info_descriptor =
        decoder_config_descriptor->decoder_specific_info_descriptor();
    auto specific_info = decoder_specific_info_descriptor->specific_info();

    uint8_t mpeg_audio_object_type = ((specific_info[0] >> 3) & 0x1f);

    // TTTT TXXX XXX  potentially 6 bits of extension.
    if (mpeg_audio_object_type == 0x1f)
    {
        if (specific_info.size() < 2)
        {
            return 0;
        }

        mpeg_audio_object_type = 32 +
            (((specific_info[0] & 0x7) << 3) | ((specific_info[1] >> 5) & 0x7));
    }

    return mpeg_audio_object_type;
}
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
                            petro::box::stsd,
                            petro::box::stsc,
                            petro::box::stco,
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
    auto mp4a = std::dynamic_pointer_cast<const petro::box::stsd::audio_sample_entry>(
        root->get_child("mp4a"));
    assert(mp4a != nullptr);

    auto sampling_frequency_index = get_sampling_frequency_index(mp4a);

    auto esds = std::dynamic_pointer_cast<const petro::box::esds>(
        mp4a->get_child("esds"));
    assert(esds != nullptr);

    auto mpeg_audio_object_type = get_mpeg_audio_object_type(esds);

    std::cout << "sampling_frequency_index " << (uint32_t) sampling_frequency_index << std::endl;
    std::cout << "mpeg_audio_object_type " << (uint32_t) mpeg_audio_object_type << std::endl;

    std::cout << mp4a->describe() << std::endl;
    for (auto c : mp4a->children())
    {
        std::cout << c->describe() << std::endl;
    }

    auto trak = mp4a->get_parent("trak");
    assert(trak != nullptr);

    auto stco = std::dynamic_pointer_cast<const petro::box::stco>(
        trak->get_child("stco"));
    assert(stco != nullptr);

    auto stsc = std::dynamic_pointer_cast<const petro::box::stsc>(
        trak->get_child("stsc"));
    assert(stsc != nullptr);

    auto stsz = std::dynamic_pointer_cast<const petro::box::stsz>(
        trak->get_child("stsz"));
    assert(stsz != nullptr);

    // create output file
    std::ofstream aac_file(argv[2], std::ios::binary);

    std::ifstream mp4_file(filename, std::ios::binary);
    auto found_samples = 0;
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        mp4_file.seekg(stco->chunk_offset(i));
        for (uint32_t j = 0; j < stsc->samples_for_chunk(i); ++j)
        {
            auto sample_size = stsz->sample_size(found_samples);
            std::vector<char> temp(sample_size);
            mp4_file.read(temp.data(), sample_size);
            aac_file.write(temp.data(), sample_size);
            found_samples += 1;
        }
    }

    aac_file.close();

    return 0;
}
