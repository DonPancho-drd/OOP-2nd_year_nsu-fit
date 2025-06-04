#include <iostream>
#include <string>
#include <fstream>
#include "sound_processor.h"

struct Arguments {
    std::string configFile;
    std::string outputFile;
    std::vector<std::string> inputFiles;
};

Arguments parseArguments(int argc, char *argv[]) {
    Arguments parsed_args;

    if (argc > 2 && std::string(argv[1]) == "-c") {
        parsed_args.configFile = argv[2];
        parsed_args.outputFile = argv[3];

        for (int i = 4; i < argc; ++i) {
            parsed_args.inputFiles.push_back(argv[i]);
        }
    } else {
        throw std::invalid_argument("Invalid arguments. Use -h for help.");
    }

    return parsed_args;
}

int main(int argc, char *argv[]) {

    if (argc < 2 || std::string(argv[1]) == "-h") {
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Usage: sound_processor [-h] [-c config.txt output.wav" << " "
                  << "input1.wav [input2.wav …]]" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Available converters:" << std::endl;
        std::cout << "    mute <start> <end>" << std::endl;
        std::cout << "    mix <input_file_ref($number)> <pos>" << std::endl;
        std::cout << "    fade <ratio(0, 50)>" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        return 0;
    }

    try {
        Arguments args = parseArguments(argc, argv);
        SoundProcessor processor(args.inputFiles, args.configFile, args.outputFile);
        processor.mainProcess();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
