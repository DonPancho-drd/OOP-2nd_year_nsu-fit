#pragma once

#include <sstream>
#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

template <typename... Args>
class CSVParser
{
private:
    std::ifstream &file;
    int skip_lines;
    char escape_char;
    std::tuple<Args...> current_tuple;
    bool is_eof = false;

    char column_separator;
    char row_separator;

    int curline_num = 0; // current line

public:
    CSVParser(std::ifstream &file, char column_separator = ',', char row_separator = '\n', char escape_char = '"', int skip_lines = 0)
        : file(file),
          column_separator(column_separator),
          row_separator(row_separator),
          escape_char(escape_char),
          skip_lines(skip_lines)
    {
        if (skip_lines < 0)
        {
            throw std::runtime_error("The value of the skip_lines must be non-negative.");
        }

        for (int i = 0; i < skip_lines; ++i)
        {
            std::string line;
            std::getline(file, line);
            if (file.eof())
            {
                is_eof = true;
                std::cout << "End of file reached while skipping " << i + 1 << " lines out of " << skip_lines << std::endl;
                break;
            }
        }

        ++(*this); // parse the first line
    }

    CSVParser begin()
    {
        return *this;
    }

    CSVParser end()
    {
        is_eof = true;
        return *this;
    }

    bool operator!=(const CSVParser & other) const
    {
        return other.is_eof != (*this).is_eof;
    }

    const std::tuple<Args...> &operator*() const
    {
        return current_tuple;
    }

    CSVParser &operator++()
    {
        std::string line;
        if (std::getline(file, line, row_separator))
        {
            ++curline_num;
            parseLine(line); // parse the line, saving it in current_tuple
        }
        else
        {
            is_eof = true;
        }

        return *this;
    }

     void parseLine(const std::string &line)
    {
        auto elements = splitLine(line); // split the line into vector of string values

        if (elements.size() != sizeof...(Args))
        {
            throw std::runtime_error("The number of values in line " + std::to_string(curline_num) +
                                     " does not match the number of types in the tuple.");
        }

        bool t = convertToTuple(elements, std::index_sequence_for<Args...>{});

        if (!t)
        {
            throw std::runtime_error("Conversion error in line " + std::to_string(curline_num));
        }
    }

    std::vector<std::string> splitLine(const std::string &line)
    {

        std::vector<std::string> result;
        std::string element;
        std::stringstream ss(line);

        while (std::getline(ss, element, column_separator))
        {
            if (element.empty())
            {
                throw std::runtime_error("Empty element in line " + std::to_string(curline_num) + " in column " + std::to_string(result.size() + skip_lines));
            }
            if (element.front() == escape_char && element.back() == escape_char)
            {
               // std::cout << "removed" << std::endl;
                element = element.substr(1, element.size() - 2);
            }
            result.push_back(element);
        }

        return result;
    }


    template <std::size_t... Is>
    bool convertToTuple(const std::vector<std::string> &elements, std::index_sequence<Is...>)
    {

        return ((fillTuple(elements[Is], std::get<Is>(current_tuple), Is + 1)), ...);
    }

    template <typename T>
    bool fillTuple(const std::string &element_str, T &tuple_value, int column)
    {
        std::istringstream iss(element_str);
        if constexpr (std::is_same_v<T, std::string>)
        {
            tuple_value = element_str;
            return true;
        }
        if (!(iss >> tuple_value))
        {
            throw std::runtime_error("Conversion error in line " + std::to_string(curline_num) + ", column " + std::to_string(column) + ": " + element_str);
            return false;
        }

        return true;
    }
};
