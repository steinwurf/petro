// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/parser.hpp>
#include <petro/box/all.hpp>

#include <fstream>
#include <string>
#include <memory>

namespace
{
    // supported mpeg versions (?)
    uint8_t MPEG_4_VERSION = 0;
    uint8_t MPEG_2_VERSION = 1;

    /// Creates an Audio Data Transport Stream (ADTS) header which is to be
    /// placed before every AAC sample.
    /// Header consists of 7 bytes.
    /// Structure
    /// AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP
    ///
    /// Letter Bits Description
    /// A      12   syncword 0xFFF, all bits must be 1
    /// B      1    MPEG Version: 0 for MPEG-4, 1 for MPEG-2
    /// C      2    Layer: always 0
    /// D      1    protection absent, Warning, set to 1 if there is no CRC
    ///             and 0 if there is CRC
    /// E      2    profile, the MPEG-4 Audio Object Type minus 1
    /// F      4    MPEG-4 Sampling Frequency Index (15 is forbidden)
    /// G      1    private bit, guaranteed never to be used by MPEG, set to 0
    ///             when encoding, ignore when decoding
    /// H      3    MPEG-4 Channel Configuration (in the case of 0, the channel
    ///             configuration is sent via an inband PCE)
    /// I      1    originality, set to 0 when encoding, ignore when decoding
    /// J      1    home, set to 0 when encoding, ignore when decoding
    /// K      1    copyrighted id bit, the next bit of a centrally registered
    ///             copyright identifier, set to 0 when encoding, ignore when
    ///             decoding.
    /// L      1    copyright id start, signals that this frame's copyright id
    ///             bit is the first bit of the copyright id, set to 0 when
    ///             encoding, ignore when decoding.
    /// M      13   frame length, this value must include 7 or 9 bytes of header
    ///             length: FrameLength =
    ///             (ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
    /// O      11   Buffer fullness, can be ignored when decoding.
    /// P      2    Number of AAC frames (raw data blocks) in ADTS frame
    ///             minus 1, for maximum compatibility always use 1 AAC frame
    ///             per ADTS frame.
    /// Q      16   CRC if protection absent is 0 (not shown in structure)
    ///
    /// Source: http://wiki.multimedia.cx/index.php?title=ADTS
    ///
    std::vector<uint8_t> create_adts(
        uint16_t aac_frame_length,
        uint8_t channel_configuration,
        uint8_t frequency_index,
        uint8_t mpeg_audio_object_type,
        uint8_t mpeg_version = MPEG_4_VERSION,
        uint8_t number_of_raw_data_blocks = 1)
    {
        assert(mpeg_version == MPEG_4_VERSION || mpeg_version == MPEG_2_VERSION);
        uint8_t protection_absent = 1;

        std::vector<uint8_t> adts;

        uint8_t byte1 = 0xFF;
        adts.push_back(byte1);

        uint8_t byte2 = 0xF0;
        byte2 |= mpeg_version << 3;
        byte2 |= protection_absent << 0;

        adts.push_back(byte2);

        uint8_t byte3 = 0x00;
        byte3 |= (mpeg_audio_object_type - 1) << 6;
        byte3 |= frequency_index << 2;

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

        return adts;
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
    auto mp4a = root->get_child("mp4a");
    assert(mp4a != nullptr);

    auto esds = std::dynamic_pointer_cast<const petro::box::esds>(
        mp4a->get_child("esds"));
    assert(esds != nullptr);
    auto decoder_config_descriptor =
        esds->descriptor()->decoder_config_descriptor();

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

    // fill output file with data.
    std::ifstream mp4_file(filename, std::ios::binary);
    uint32_t found_samples = 0;
    for (uint32_t i = 0; i < stco->entry_count(); ++i)
    {
        mp4_file.seekg(stco->chunk_offset(i));
        for (uint32_t j = 0; j < stsc->samples_for_chunk(i); ++j)
        {
            uint16_t sample_size = stsz->sample_size(found_samples);

            auto adts = create_adts(
                sample_size,
                decoder_config_descriptor->channel_configuration(),
                decoder_config_descriptor->frequency_index(),
                decoder_config_descriptor->mpeg_audio_object_type());
            aac_file.write((char*)adts.data(), adts.size());

            std::vector<char> temp(sample_size);
            mp4_file.read(temp.data(), sample_size);
            aac_file.write(temp.data(), sample_size);
            found_samples += 1;
        }
    }

    aac_file.close();

    return 0;
}
