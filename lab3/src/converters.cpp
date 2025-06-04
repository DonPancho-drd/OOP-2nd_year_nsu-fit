#include "converters.h"
#include <iostream>
#include "wav_file.h"

MuteConverter::MuteConverter(int start_time, int end_time)
    : start(start_time), end(end_time) {}

void MuteConverter::apply(std::vector<int16_t> &samples)
{
    int start_sample = start * WavFile::sampleRate;
    int end_sample = end * WavFile::sampleRate;

    if (start_sample > samples.size() || start_sample >= end_sample || end_sample > samples.size())
    {
        std::cout << start << " " << end << std::endl;
        throw std::runtime_error("Invalid input for mute converter");
    }


    for (int i = start_sample; i < end_sample; i++)
    {
        samples[i] = 0;
    }
}

MixConverter::MixConverter(const std::string &input_file2, int insert_pos)
    : pos(insert_pos)
{
    WavFile file(input_file2);
    file.read();
    mix_samples = std::move(file.getSamples());
}

void MixConverter::apply(std::vector<int16_t> &samples)
{
    int insert_sample = pos * WavFile::sampleRate;

    for (int i = 0; i < mix_samples.size() && (insert_sample + i) < samples.size(); i++)
    {
        samples[insert_sample + i] = (samples[insert_sample + i] + mix_samples[i]) / 2;
    }
}




FadeInOutConverter::FadeInOutConverter(int ratio)
    : ratio(ratio) {}
void FadeInOutConverter::apply(std::vector<int16_t> &samples)
{
    int fade_samples = samples.size() * ratio / 100;

    for (int i = 0; i < fade_samples; i++)
    {
        samples[i] = static_cast<int16_t>(samples[i] * static_cast<float>(i) / fade_samples);

        samples[samples.size() - i - 1] = static_cast<int16_t>(samples[samples.size() - i - 1] * static_cast<float>(i) / fade_samples);
    }
}
