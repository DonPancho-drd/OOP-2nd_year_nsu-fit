#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "strategy_factory.h"
#include "game.h"
#include "args_parser.h"

void playDetailed(std::vector<std::shared_ptr<Strategy>> strategies, std::array<std::array<int, 3>, 8> matrix, std::unique_ptr<Arguments> &args)
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
            break;
        }
    }

    game.printResults();
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    auto args = parse_arguments(argc, argv);

    std::array<std::array<int, 3>, 8> matrix = {0}; // matrix

    if (!(args->matrix_file.empty()))
    {

        matrix = load_matrix(args->matrix_file);
    }

    std::vector<std::shared_ptr<Strategy>> strategies; // strategies

    for (auto &name : args->players)
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

    if (GameMode::Detailed == args->mode)
    {
        playDetailed(strategies, matrix, args);
    }

    if(GameMode::Tournament == args->mode){
        int player_cnt = strategies.size();
        for(int i = 0; i < player_cnt; i++){
            for(int j = i; j < player_cnt; j++){
                for(int k = j; k < player_cnt; k++){

                }
            }
        }

    }

    return EXIT_SUCCESS;
}
