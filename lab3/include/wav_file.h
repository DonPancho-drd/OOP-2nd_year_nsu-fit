#pragma once

#include <cstdint>
#include <string>
#include <vector>

class WavFile
{
private:
    std::string filename;
    std::vector<int16_t> samples;
    void parseHeaderAndFormat(std::ifstream &file);

public:
    static const int sampleRate = 44100;
    WavFile(const std::string &filename);
    void read();
    void write(const std::string &filename);
    const std::vector<int16_t> &getSamples() { return samples; }
    int getSampleRate() const { return sampleRate; }
    void setSamples(std::vector<int16_t> &&samples) { this->samples = samples; }
};
