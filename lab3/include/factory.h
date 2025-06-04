#include <iostream>
#include <memory>
#include "converters.h"

class ConverterFactory {
    ~ConverterFactory() = delete;
public:
    static std::unique_ptr<Converter> createConverter(const std::string& cmd,
                                             const std::vector<std::string>& input_files);
};
