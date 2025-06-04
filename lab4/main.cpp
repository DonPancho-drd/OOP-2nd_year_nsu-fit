#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include "csv_parser.h"

template <typename... Args, std::size_t... Is>
void print_tuple(std::ostream &os, const std::tuple<Args...> &t, std::index_sequence<Is...>)
{
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}


template <typename... Args>
std::ostream & operator<<(std::ostream &os, const std::tuple<Args...> &t)
{
    os << "(";
    print_tuple(os, t, std::index_sequence_for<Args...>{});
    os << ")";
    return os;
}

int main()
{

    std::ifstream file("w.csv");

    if (!file.is_open())
    {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    CSVParser<int, std::string, std::string, double> parser(file, ',', '\n', '"', 2);
    for (const auto &row : parser)
    {
        std::cout << row << std::endl;
    }

    return 0;
}
