#include <gtest/gtest.h>
#include "config_parser.h"
#include <fstream>
#include "wav_file.h"
#include "sound_processor.h"

TEST(ConfigParserTests, FileNotFound) {
    ConfigParser parser("non_existent_file.cfg");
    EXPECT_THROW(parser.parseFilesToConverters({"../loop.wav"}), std::runtime_error);
}

TEST(ConfigParserTests, UnknownAction) {
    std::ofstream ofs("conf.txt");
    ofs << "unknown_action\n";
    ofs.close();

    ConfigParser parser("conf.txt");
    EXPECT_THROW(parser.parseFilesToConverters({"../loop.wav"}), std::runtime_error);
}

TEST(ConfigParserTests, InvalidMixConverterInput) {
    std::ofstream ofs("conf.txt");
    ofs << "mix $10 100\n"; // Invalid index
    ofs.close();

    ConfigParser parser("conf.txt");
    EXPECT_THROW(parser.parseFilesToConverters({"../loop.wav"}), std::runtime_error);
}

TEST(ConfigParserTests, InvalidFadeRatio) {
    std::ofstream ofs("conf.txt");
    ofs << "fade 51\n"; // Invalid ratio
    ofs.close();

    ConfigParser parser("conf.txt");
    EXPECT_THROW(parser.parseFilesToConverters({"../loop.wav"}), std::invalid_argument);
}




TEST(WavFileTests, FileNotFound) {
    WavFile wav("non_existent_file.wav");
    EXPECT_THROW(wav.read(), std::runtime_error);
}

TEST(WavFileTests, UnsupportedFormat) {
    std::ofstream ofs("invalid_format.wav", std::ios::binary);
    ofs << "INVALID"; // Writing an invalid header
    ofs.close();

    WavFile wav("invalid_format.wav");
    EXPECT_THROW(wav.read(), std::runtime_error);
}

TEST(WavFileTests, InvalidFormatChunk) {
    std::ofstream ofs("invalid_format_chunk.wav", std::ios::binary);
    ofs << "RIFF" << (uint32_t)0 << "WAVE" << "INVALID"; // Invalid format chunk
    ofs.close();

    WavFile wav("invalid_format_chunk.wav");
    EXPECT_THROW(wav.read(), std::runtime_error);
}

TEST(WavFileTests, UnsupportedAudioFormat) {
    std::ofstream ofs("unsupported_audio_format.wav", std::ios::binary);
    ofs << "RIFF" << (uint32_t)0 << "WAVE" << "fmt " << (uint32_t)16 << (uint16_t)3; // Unsupported audio format (3 channels)
    ofs.close();

    WavFile wav("unsupported_audio_format.wav");
    EXPECT_THROW(wav.read(), std::runtime_error);
}

TEST(WavFileTests, DataChunkReadError) {
    std::ofstream ofs("data_chunk_read_error.wav", std::ios::binary);
    ofs << "RIFF" << (uint32_t)0 << "WAVE" << "fmt " << (uint32_t)16 << (uint16_t)1 << (uint16_t)1 << (uint32_t)44100 << (uint32_t)88200 << (uint16_t)2 << (uint16_t)16;
    ofs << "data" << (uint32_t)3; // Malformed data chunk (not enough data)
    ofs.close();

    WavFile wav("data_chunk_read_error.wav");
    EXPECT_THROW(wav.read(), std::runtime_error);
}

TEST(WavFileTests, WriteError) {
    WavFile wav("invalid_directory/output.wav");
    EXPECT_THROW(wav.write("invalid_directory/output.wav"), std::runtime_error);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
