#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <array>
#include "strategy_factory.h"
#include "game.h"
#include "args_parser.h"

std::tuple<int, int, int> playDetailed(const std::array<std::array<int, 3>, 8> &matrix, const std::vector<std::shared_ptr<Strategy>> &strategies, const std::unique_ptr<Arguments> &args)
{
    auto a = strategies[0];
    auto b = strategies[1];
    auto c = strategies[2];

    std::cout << "GAME: " << a->getStrategyName() << " vs " << b->getStrategyName() << " vs " << c->getStrategyName() << std::endl;

    PrisonerDillema game(a, b, c, matrix);

    for (int i = 0; i < args->steps; ++i)
    {
        auto [move_a, move_b, move_c] = game.playRound();
        std::cout << "Step " << i + 1 << std::endl;

        std::cout << (move_a == Move::COOPERATE ? "C" : "D") << "\t:\t"
                  << (move_b == Move::COOPERATE ? "C" : "D") << "\t:\t"
                  << (move_c == Move::COOPERATE ? "C" : "D") << std::endl;

        const auto &scores = game.getScores();
        std::cout << std::get<0>(scores) << "\t:\t"
                  << std::get<1>(scores) << "\t:\t"
                  << std::get<2>(scores) << std::endl;

        // Выход по кнопке 'q'
        if (getchar() == 'q')
        {
            getchar();
            break;
        }
    }

    game.printResults();
    std::cout << std::endl;

    return game.getScores();
}

int main(int argc, char **argv)
{
    auto args = parse_arguments(argc, argv);

    std::array<std::array<int, 3>, 8> matrix = {}; // Инициализация массива нулями

    if (!args->matrix_file.empty())
    {
        matrix = load_matrix(args->matrix_file);
    }

    std::vector<std::shared_ptr<Strategy>> strategies; // strategies

    for (auto name : args->players)
    {
        strategies.push_back(StrategyFactory::createStrategy(name));
    }

    if (args->mode == GameMode::NoDetailed)
    {
        PrisonerDillema game(strategies[0], strategies[1], strategies[2], matrix);

        for (int i = 0; i < args->steps; i++)
        {
            game.playRound();
        }
        game.printResults();
        return EXIT_SUCCESS;
    }

    if (args->mode == GameMode::Detailed)
    {
        playDetailed(matrix, strategies, args);
    }

    if (args->mode == GameMode::Tournament)
    {
        std::cout << "--------------------TOURNAMENT--------------------" << std::endl;
        std::cout << std::endl;
        std::tuple<int, int, int> res;
        int player_cnt = strategies.size();
        std::vector<int> tourney_scores(player_cnt, 0);

        for (int i = 0; i < player_cnt; i++)
        {
            for (int j = i + 1; j < player_cnt; j++)
            {
                for (int k = j + 1; k < player_cnt; k++)
                {
                    res = playDetailed(matrix, {strategies[i], strategies[j], strategies[k]}, args);
                    tourney_scores[i] += std::get<0>(res);
                    tourney_scores[j] += std::get<1>(res);
                    tourney_scores[k] += std::get<2>(res);

                    while (true)
                    {
                        std::cout << "Press 'c' to continue or 'q' to quit" << std::endl;
                        char input = getchar();
                        if (input == 'c')
                        {
                            getchar();
                            break; // Продолжить к следующему матчу
                        }
                        else if (input == 'q')
                        {
                            std::cout << "Game over by user" << std::endl;
                            return EXIT_SUCCESS;
                        }
                    }
                }
            }
        }

        std::cout << "-----------------ღ(◕‿◕｡)ღ ✧(✿❛‿❛✿)✧ ★(｡♥‿♥｡)★-----------------" << std::endl;

        for (int i = 0; i < player_cnt; i++)
        {
            std::cout << strategies[i]->getStrategyName() << " score: " << tourney_scores[i] << std::endl;
        }

        std::cout << "--------------------(¬‿¬)  (｡♥‿♥｡)  (ノ°∀°)ノ---------------------" << std::endl;
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
