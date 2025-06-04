#include "sound_processor.h"
#include <iostream>

#include "factory.h"

SoundProcessor::SoundProcessor(const std::vector<std::string> &input, const std::string &config,
                               const std::string &output)

    : input_files(input), config_file(config), output_file(output) {}


void SoundProcessor::mainProcess() {

    ConfigParser parser(config_file);
    auto commands = parser.splitConfigs(input_files.size());

    WavFile input(input_files[0]);
    input.read();
    auto output_samples = input.getSamples();

    for (const auto &cmd : commands) {
        auto converter = ConverterFactory::createConverter(cmd, input_files);
        converter->apply(output_samples);
    }

    WavFile output(output_file);
    output.setSamples(std::move(output_samples));
    output.write(output_file);
}
