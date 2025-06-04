#include "wav_file.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdint>

struct WavHeader
{
    char riff[4];      // "RIFF" - идентификатор формата файла
    uint32_t fileSize; // Размер файла в байтах, исключая первые 8 байт (RIFF и размер файла)
    char wave[4];      // "WAVE" - идентификатор формата WAV (тип riff)
};


struct WavFmtChunk
{
    char fmt[4];            // "fmt " - идентификатор блока формата
    uint32_t fmtSize;       // Размер блока формата (16 для PCM)
    uint16_t audioFormat;   // Формат аудио (1 для PCM)
    uint16_t numChannels;   // Количество каналов (1 = моно)
    uint32_t sampleRate;    // Частота дискретизации (44100 Гц)
    uint32_t byteRate;      // Количество байт, передаваемых в секунду (SampleRate * NumChannels * BitsPerSample / 8)
    uint16_t blockAlign;    // Размер блока (каналы*байты в )
    uint16_t bitsPerSample; // Количество бит на сэмпл
};

struct WavDataChunk
{
    char data[4];      // "data" - идентификатор блока данных
    uint32_t dataSize; // Размер блока данных
};

WavFile::WavFile(const std::string &filename)
    : filename(filename) {}

void WavFile::read()
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Error opening WAV file for reading.");
    }

    parseHeaderAndFormat(file); // read header and format chunk

    WavDataChunk dataChunk;

    // Skip all chunks except "data"
    while (true)
    {

        file.read(reinterpret_cast<char *>(&dataChunk), sizeof(WavDataChunk));
        if (!file)
        {
            throw std::runtime_error("Error reading WAV data chunk from file.");
        }

        if (std::string(dataChunk.data, 4) == "data")
        {
            break;
        }
    }

    if (dataChunk.dataSize % sizeof(int16_t) != 0)
    {
        throw std::runtime_error("Error pasing dataChunk from WAV file.");
    }

    // reading samples
    samples.resize(dataChunk.dataSize / sizeof(int16_t)); // number of samples
    file.read(reinterpret_cast<char *>(samples.data()), dataChunk.dataSize);

    if (!file)
    {
        throw std::runtime_error("Error reading data.");
    }

}

void WavFile::parseHeaderAndFormat(std::ifstream &file)
{
    WavHeader header;
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!file)
    {
        throw std::runtime_error("Error reading header from file.");
    }

    if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE")
    {
        throw std::runtime_error("Unsupported format.");
    }

    WavFmtChunk fmtChunk;
    file.read(reinterpret_cast<char *>(&fmtChunk), sizeof(fmtChunk));
    if (!file)
    {
        throw std::runtime_error("Error reading format chunk from file.");
    }

    if (std::string(fmtChunk.fmt, 4) != "fmt ")
    {
        std::cout << std::string(fmtChunk.fmt, 4) << std::endl;
        throw std::runtime_error("Invalid format chunk.");
    }

    // Checking that the format is PCM
    if (fmtChunk.audioFormat != 1 || fmtChunk.numChannels != 1 || fmtChunk.sampleRate != 44100 || fmtChunk.bitsPerSample != 16)
    {
        throw std::runtime_error("Unsupported format.");
    }
}

void WavFile::write(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Error opening WAV file for writing.");
    }

    uint32_t dataSize = samples.size() * sizeof(int16_t);

    WavFmtChunk fmtChunk = {
        {'f', 'm', 't', ' '}, // Идентификатор блока формата
        16,                   // fmtSize
        1,                    // udioFormat
        1,                    // numChannels
        44100,                // sampleRate
        44100 * 16 / 8,       // byteRate
        2,                    // blockAlign
        16                    // bitsPerSample
    };


    WavDataChunk dataChunk = {
        {'d', 'a', 't', 'a'},
        dataSize};

    WavHeader header = {
        {'R', 'I', 'F', 'F'},
        static_cast<uint32_t>((sizeof(header.wave)) + (sizeof(WavFmtChunk)) + (sizeof(WavDataChunk)) + dataSize),
        {'W', 'A', 'V', 'E'}};

    file.write(reinterpret_cast<char *>(&header), sizeof(header));
    file.write(reinterpret_cast<char *>(&fmtChunk), sizeof(fmtChunk));
    file.write(reinterpret_cast<char *>(&dataChunk), sizeof(dataChunk));
    file.write(reinterpret_cast<char *>(samples.data()), dataSize);
}
