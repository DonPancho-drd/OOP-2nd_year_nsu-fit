#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <array>

enum class GameMode
{
    Detailed,
    NoDetailed,
    Tournament
};

struct Arguments
{
    std::vector<std::string> players;
    GameMode mode = GameMode::Detailed;
    int steps = 50;
    std::string directory_configs;
    std::string matrix_file;   // вшита в game.h
};

static std::unique_ptr<Arguments> parse_arguments(int argc, char *argv[]);

std::array<std::array<int, 3>, 8> load_matrix(const std::string &filename);
