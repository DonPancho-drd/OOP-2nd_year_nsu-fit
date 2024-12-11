#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <tuple>
#include "strategy_factory.h"

class PrisonerDillema {
private:
    std::array<std::shared_ptr<Strategy>, 3> strategies;
    std::array<int, 3> result_scores;
    std::array<std::array<int, 3>, 8> payoff_matrix;
public:

    PrisonerDillema(std::shared_ptr<Strategy> &strategy1, std::shared_ptr<Strategy> &strategy2,
    std::shared_ptr<Strategy> &strategy3,
    const std::array<std::array<int, 3>, 8> &matrix);

    std::tuple<Move, Move, Move> playRound();
    std::tuple<int, int, int> getScores() const;
    void printResults() const;

};
