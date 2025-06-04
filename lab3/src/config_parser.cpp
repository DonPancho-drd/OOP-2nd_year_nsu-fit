#include "config_parser.h"

#include <fstream>
#include <iostream>
#include <sstream>


ConfigParser::ConfigParser(const std::string &filename) : filename(filename) {}

std::vector<std::string> ConfigParser::splitConfigs(int input_files_count)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open config file");
    }

    std::vector<std::string> commands;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        std::string cmd;
        commands.push_back(line);
        }

    return commands;
}

// #include "config_parser.h"

// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <tuple>

// ConfigParser::ConfigParser(const std::string &filename) : filename(filename) {}

// std::vector<std::tuple<std::string, int, int>> ConfigParser::parseConfigs()
// {
//     std::ifstream file(filename);
//     if (!file)
//     {
//         throw std::runtime_error("Failed to open config file");
//     }

//     std::vector<std::string> converters;
//     std::string line;
//     while (std::getline(file, line))
//     {
//         if (line.empty() || line[0] == '#')
//             continue;
//         std::istringstream iss(line);
//         std::string action;
//         iss >> action;

//         if (action == "mute")
//         {
//             int start, end;
//             iss >> start >> end;
//             converters.push_back(std::make_unique<MuteConverter>(start, end));
//         }

//         else if (action == "mix")
//         {
//             int pos;
//             std::string file_ref;
//             iss >> file_ref >> pos;
//             int file_index = std::stoi(file_ref.substr(1, file_ref.size() - 1));
//             if (file_ref[0] != '$' || file_index < 1 || file_index > input_files.size())
//             {
//                 throw std::runtime_error("Invalid input for mix converter");
//             }

//             converters.push_back(std::make_unique<MixConverter>(input_files[file_index - 1], pos));
//         }
//         else if (action == "fade")
//         {
//             int ratio;
//             iss >> ratio;
//             if (ratio <= 0 || ratio >= 50)
//             {
//                 throw std::invalid_argument("ratio must be between 0 and 50 percets");
//             }
//             converters.push_back(std::make_unique<FadeInOutConverter>(ratio));
//         }
//         else
//         {
//             throw std::runtime_error("Unknown action in config file");
//         }
//     }
//     return converters;
// }
