
#include "args_parser.h"




static std::unique_ptr<Arguments> parse_arguments(int argc, char *argv[])
{
    auto args = std::make_unique<Arguments>();
    if (argc < 4)
    {
        throw std::invalid_argument("Not enough arguments");
    }

    for (int i = 1; i < argc; ++i)
    {
        std::string argument = argv[i];
        if (argument.substr(0, 2) == "--")
        {
            if (argument.substr(0, 7) == "--mode=")
            {
                std::string mode = argument.substr(7);
                if (mode == "detailed")
                {
                    args->mode = GameMode::Detailed;
                }
                else if (mode == "fast")
                {
                    args->mode = GameMode::NoDetailed;
                }
                else if (mode == "tournament")
                {
                    args->mode = GameMode::Tournament;
                }
                else
                {
                    throw std::invalid_argument("Unknown mode: " + mode);
                }
            }
            else if (argument.substr(0, 8) == "--steps=")
            {
                args->steps = std::stoi(argument.substr(8));
                if (args->steps < 1 || args->steps > 50)
                {
                    throw std::invalid_argument("Steps must be between 1 and 50.");
                }
            }
            else if (argument.substr(0, 13) == "--config-dir=")
            {
                args->directory_configs = argument.substr(13);
            }
            else if (argument.substr(0, 9) == "--matrix=")
            {
                args->matrix_file = argument.substr(9);
            }
            else
            {
                throw std::invalid_argument("Unknown argument: " + argument);
            }
        }
        else
        {
            args->players.push_back(argument);
        }
    }

    if (args->players.size() < 3)
    {
        throw std::invalid_argument("At least 3 strategies are required.");
    }
    if (args->players.size() > 3 && args->mode != GameMode::Tournament)
    {
        throw std::invalid_argument("Only tournament can be played by more than 3 players.");
    }

    return args;
}

std::array<std::array<int, 3>, 8> load_matrix(const std::string &filename)
{
    std::array<std::array<int, 3>, 8> matrix{};
    if (!filename.empty())
    {
        std::ifstream file(filename);
        if (!file)
            throw std::runtime_error("Failed to open matrix file.");
        for (auto &row : matrix)
        {
            for (auto &elem : row)
            {
                file >> elem;
            }
        }
    }

    return matrix;
}
