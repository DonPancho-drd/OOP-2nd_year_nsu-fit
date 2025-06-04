#include "factory.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::unique_ptr<Converter> ConverterFactory::createConverter(const std::string& cmd, const std::vector<std::string>& input_files) {

        std::istringstream iss(cmd);
        std::string action;
        iss >> action;

        if (action == "mute")
        {
            int start, end;
            iss >> start >> end;
            if(start < 0 || start > end || end < 0 ){
                throw std::runtime_error("Invalid input for mute converter");
            }
            return std::make_unique<MuteConverter>(start, end);
        }

        else if (action == "mix")
        {
            int pos;
            std::string file_ref;
            iss >> file_ref >> pos;
            int file_index = std::stoi(file_ref.substr(1, file_ref.size() - 1));
            if (file_ref[0] != '$' || file_index < 1 || file_index > input_files.size())
            {
                throw std::runtime_error("Invalid input for mix converter");
            }

            return std::make_unique<MixConverter>(input_files[file_index - 1], pos);
        }
        else if (action == "fade")
        {
            int ratio;
            iss >> ratio;
            if (ratio <= 0 || ratio >= 50)
            {
                throw std::invalid_argument("Ratio must be between 0 and 50 percets");
            }
            return std::make_unique<FadeInOutConverter>(ratio);
        }
        else
        {
            throw std::runtime_error("Unknown action in config file");
        }


}
