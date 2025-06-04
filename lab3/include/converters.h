#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Converter {
public:
    virtual ~Converter() = default;
    virtual void apply(std::vector<int16_t> &samples) = 0;
};


class MuteConverter : public Converter {
    int start, end;
public:
    MuteConverter(int start_time, int end_time);
    void apply(std::vector<int16_t> &samples) override;
};



class MixConverter : public Converter {
    std::vector<int16_t> mix_samples;
    int pos;
public:
    MixConverter(const std::string &input_file2, int insert_pos);
    void apply(std::vector<int16_t> &samples) override;
};



class FadeInOutConverter : public Converter {
    int ratio;

public:
    FadeInOutConverter(int ratio);
    void apply(std::vector<int16_t> &samples) override;
};
