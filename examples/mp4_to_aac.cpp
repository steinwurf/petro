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

std::vector<uint8_t> create_adts(
    std::shared_ptr<const petro::box::esds> esds, uint16_t aac_frame_length,
    uint8_t number_of_raw_data_blocks = 1)
{
    auto descriptor = esds->descriptor()->decoder_config_descriptor();

    std::vector<uint8_t> adts;
    adts.push_back(0xFF); // syncword 0xFF, all bits must be 1 // byte1
    // ____
    // 1111 0000 syncword 0xF0, all bits must be 1
    //      _
    // 0000 0000 (assuming) mpeg 4
    //       __
    // 0000 0000 Layer: always 0
    //         _
    // 0000 0001 protection absent, 1 if there is no CRC and 0 if there is CRC
    // anded, this gives: 11110001 = 0xF1
    adts.push_back(0xF1); // byte2 MPEG-4
    // adts.push_back(0xF9); // byte2 MPEG-2
    uint8_t byte3 = 0;

    byte3 |= (descriptor->mpeg_audio_object_type() - 1) << 6;
    byte3 |= descriptor->frequency_index() << 2;
    auto channel_configuration = descriptor->channel_configuration();
    byte3 |= (channel_configuration & 0x04) >> 2;
    adts.push_back(byte3);

    uint8_t byte4 = 0;

    byte4 |= (channel_configuration & 0x03) << 6;
    // frame length, this value must include the 7 bytes of header length
    uint16_t frame_length = aac_frame_length + 7;
    assert(frame_length <= 0x1FFF);
    byte4 |= (frame_length & 0x1800) >> 11;

    adts.push_back(byte4);

    adts.push_back((frame_length & 0x07F8) >> 3); // byte5

    uint8_t byte6 = 0xFF;
    byte6 &= (frame_length & 0x0007) << 5;
    adts.push_back(byte6);

    uint8_t byte7 = 0xB0;
    byte7 |= (number_of_raw_data_blocks - 1) & 0x03;
    adts.push_back(byte7);

    // petro::bit_reader b(adts);

    // for (uint32_t i = 0; i < b.size(); ++i)
    // {
    //     if (i % 8 == 0)
    //         std::cout << " ";
    //     std::cout << (uint32_t) b.read_1_bit();
    // }
    // std::cout << std::endl;


    return adts;
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
    auto mp4a = root->get_child("mp4a");
    assert(mp4a != nullptr);

    auto esds = std::dynamic_pointer_cast<const petro::box::esds>(
        mp4a->get_child("esds"));
    assert(esds != nullptr);

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
    uint32_t found_samples = 0;
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        mp4_file.seekg(stco->chunk_offset(i));
        for (uint32_t j = 0; j < stsc->samples_for_chunk(i); ++j)
        {
            uint16_t sample_size = stsz->sample_size(found_samples);

            auto adts = create_adts(esds, sample_size);
            aac_file.write((char*)adts.data(), adts.size());

            std::vector<char> temp(sample_size);
            mp4_file.read(temp.data(), sample_size);
            aac_file.write(temp.data(), sample_size);
            found_samples += 1;
        }
    }
    std::cout << (uint32_t)found_samples << std::endl;

    aac_file.close();

    return 0;
}
