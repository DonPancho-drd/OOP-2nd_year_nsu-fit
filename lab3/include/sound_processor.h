#pragma once

#include <string>
#include <vector>
#include "config_parser.h"
#include "wav_file.h"

class SoundProcessor
{
public:
    SoundProcessor(const std::vector<std::string> &input, const std::string &config, const std::string &output);

    void mainProcess();

private:
    std::vector<std::string> input_files;
    std::string config_file;
    std::string output_file;
};
