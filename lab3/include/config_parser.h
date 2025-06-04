#pragma once

#include <string>
#include <vector>
#include <exception>
#include <memory>

#include "converters.h"

class ConfigParser {
public:
    explicit ConfigParser(const std::string &filename);
    std::vector<std::string> splitConfigs(int input_files_count);

private:
    std::string filename;
};
