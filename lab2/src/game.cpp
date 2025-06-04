#include "game.h"

PrisonerDillema::PrisonerDillema(std::shared_ptr<Strategy> &strategy1, std::shared_ptr<Strategy> &strategy2,
                                 std::shared_ptr<Strategy> &strategy3,
                                 const std::array<std::array<int, 3>, 8> &matrix) : strategies{strategy1, strategy2, strategy3},
                                                                                    result_scores{0, 0, 0}
{
    bool is_empty = true;
    for (const auto &row : matrix)
    {
        for (int value : row)
        {
            if (value != 0)
            {
                is_empty = false;
                break;
            }
        }
        if (!is_empty)
            break;
    }

    if (!is_empty)
    {
        payoff_matrix = matrix;
    }
    else
    {
        payoff_matrix = {
            std::array{7, 7, 7}, std::array{3, 3, 9},
            std::array{3, 9, 3}, std::array{0, 5, 5},
            std::array{9, 3, 3}, std::array{5, 0, 5},
            std::array{5, 5, 0}, std::array{1, 1, 1}};
    }
}

std::tuple<Move, Move, Move> PrisonerDillema::playRound()
{
    Move step1 = strategies[0]->makeMove();
    Move step2 = strategies[1]->makeMove();
    Move step3 = strategies[2]->makeMove();

    int row = 0;
    if (step1 == Move::DEFECT){row += 4;}        // D - 1   С - 0
    if (step2 == Move::DEFECT){row += 2;}
    if (step3 == Move::DEFECT){row += 1;}

    result_scores[0] += payoff_matrix[row][0];
    result_scores[1] += payoff_matrix[row][1];
    result_scores[2] += payoff_matrix[row][2];
    std::cout << "Scores: " << payoff_matrix[row][0] << " " << payoff_matrix[row][1] << " " << payoff_matrix[row][2] << std::endl;

    strategies[0]->applyExperience(std::make_pair(step3, step2));
    strategies[1]->applyExperience(std::make_pair(step1, step3));
    strategies[2]->applyExperience(std::make_pair(step2, step1));

    return std::make_tuple(step1, step2, step3);
}

std::tuple<int, int, int> PrisonerDillema::getScores() const
{
    return std::make_tuple(result_scores[0], result_scores[1], result_scores[2]);
}

void PrisonerDillema::printResults() const
{
    std::cout << "^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*" << std::endl;
    for (int i = 0; i < strategies.size(); i++)
    {
        std::cout << "Player " << strategies[i]->getStrategyName() << " score: " << result_scores[i] << std::endl;
    }
    std::cout << "^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*" << std::endl;
}
