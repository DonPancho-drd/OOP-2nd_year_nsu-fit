#pragma once

#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <fstream>

template <typename T>
void print_tuple_element(std::ostream& os, const T& value) {
    os << value;
}

template <std::size_t Index, typename... Args>
void print_tuple(std::ostream& os, const std::tuple<Args...>& t) {
    if constexpr (Index < sizeof...(Args)) {
        if (Index > 0) {
            os << ", ";
        }
        print_tuple_element(os, std::get<Index>(t));
        print_tuple<Index + 1>(os, t);
    }
}


template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "("; /
    print_tuple<0>(os, t);
    os << ")";
    return os;
}
