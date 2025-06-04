#include "include/game.h"
#include "include/args_parser.h"
#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <tuple>
#include <iostream>

#include "include/game.h"
#include "include/args_parser.h"

TEST(PrisonerDillemaTest, ConstructorInitializesCorrectly)
{
    std::shared_ptr<Strategy> strategy1 = StrategyFactory::createStrategy("poor_trusting_fool");
    std::shared_ptr<Strategy> strategy2 = StrategyFactory::createStrategy("all_defector");
    std::shared_ptr<Strategy> strategy3 = StrategyFactory::createStrategy("tit_for_tat");

    std::array<std::array<int, 3>, 8> matrix = {{{7, 7, 7}, {3, 3, 9}, {3, 9, 3}, {0, 5, 5}, {9, 3, 3}, {5, 0, 5}, {5, 5, 0}, {1, 1, 1}}};

    PrisonerDillema game(strategy1, strategy2, strategy3, matrix);

    auto [score1, score2, score3] = game.getScores();
    EXPECT_EQ(score1, 0);
    EXPECT_EQ(score2, 0);
    EXPECT_EQ(score3, 0);
}

TEST(PrisonerDillemaTest, PlayRoundUpdatesScores)
{
    std::shared_ptr<Strategy> strategy1 = StrategyFactory::createStrategy("poor_trusting_fool");
    std::shared_ptr<Strategy> strategy2 = StrategyFactory::createStrategy("all_defector");
    std::shared_ptr<Strategy> strategy3 = StrategyFactory::createStrategy("tit_for_tat");

    // std::array<std::array<int, 3>, 8> matrix = {{{7, 7, 7}, {3, 3, 9}, {3, 9, 3}, {0, 5, 5},
    //                                              {9, 3, 3}, {5, 0, 5}, {5, 5, 0}, {1, 1, 1}}};
    std::array<std::array<int, 3>, 8> matrix = {};

    PrisonerDillema game(strategy1, strategy2, strategy3, matrix);

    game.playRound(); // C D C
    auto [score1, score2, score3] = game.getScores();

    EXPECT_EQ(score1, 3);
    EXPECT_EQ(score2, 9);
    EXPECT_EQ(score3, 3);

    game.playRound(); // C D D
    auto [score4, score5, score6] = game.getScores();

    EXPECT_EQ(score4, 3);
    EXPECT_EQ(score5, 14);
    EXPECT_EQ(score6, 8);
}

// Проверка метода с новой матрицей
TEST(PrisonerDillemaTest, GetScoresReturnsCorrectScores)
{
    std::shared_ptr<Strategy> strategy1 = StrategyFactory::createStrategy("poor_trusting_fool");
    std::shared_ptr<Strategy> strategy2 = StrategyFactory::createStrategy("all_defector");
    std::shared_ptr<Strategy> strategy3 = StrategyFactory::createStrategy("tit_for_tat");

    std::array<std::array<int, 3>, 8> matrix = {{{70, 70, 70}, {30, 30, 90}, {30, 90, 30}, {0, 50, 50}, {90, 30, 30}, {50, 0, 50}, {50, 50, 0}, {10, 10, 10}}};

    PrisonerDillema game(strategy1, strategy2, strategy3, matrix);

    game.playRound();
    auto [score1, score2, score3] = game.getScores();

    EXPECT_EQ(score1, 30);
    EXPECT_EQ(score2, 90);
    EXPECT_EQ(score3, 30);
}

TEST(PrisonerDillemaTest, CorrectStrategyInteraction)
{
    std::shared_ptr<Strategy> strategy1 = StrategyFactory::createStrategy("periodic");
    std::shared_ptr<Strategy> strategy2 = StrategyFactory::createStrategy("all_defector");
    std::shared_ptr<Strategy> strategy3 = StrategyFactory::createStrategy("go_by_majority");

    std::array<std::array<int, 3>, 8> matrix = {};

    PrisonerDillema game(strategy1, strategy2, strategy3, matrix);

    game.playRound(); // D D C    5 5 0
    game.playRound(); // C D D    0 5 5

    auto [score1, score2, score3] = game.getScores();

    EXPECT_EQ(score1, 5);
    EXPECT_EQ(score2, 10);
    EXPECT_EQ(score3, 5);
}

TEST(ArgsParserTest, TestParseArgumentsValidInput)
{
    char *argv[] = {
        (char *)"program_name",
        (char *)"player1",
        (char *)"player2",
        (char *)"player3",
        (char *)"--mode=detailed",
        (char *)"--steps=10",
        (char *)"--config-dir=config",
        (char *)"--matrix=matrix.txt"};

    int argc = sizeof(argv) / sizeof(argv[0]);

    auto args = parse_arguments(argc, argv);

    EXPECT_EQ(args->players.size(), 3);
    EXPECT_EQ(args->players[0], "player1");
    EXPECT_EQ(args->players[1], "player2");
    EXPECT_EQ(args->players[2], "player3");
    EXPECT_EQ(args->mode, GameMode::Detailed);
    EXPECT_EQ(args->steps, 10);
    EXPECT_EQ(args->directory_configs, "config");
    EXPECT_EQ(args->matrix_file, "matrix.txt");
}

TEST(ArgsParserTest, TestParseArgumentsUnknownMode)
{
    char *argv[] = {
        (char *)"program_name",
        (char *)"player1",
        (char *)"player2",
        (char *)"player3",
        (char *)"--mode=prprprprp"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    EXPECT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(ArgsParserTest, TestParseArgumentsNotEnoughPlayers)
{
    // Подготовка аргументов с недостаточным количеством игроков
    char *argv[] = {
        (char *)"program_name",
        (char *)"player1",
        (char *)"player2"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(ArgsParserTest, TestParseArgumentsTooManyPlayersNonTournament)
{
    // Подготовка аргументов с более чем 3 игроками в не-турнирном режиме
    char *argv[] = {
        (char *)"program_name",
        (char *)"player1",
        (char *)"player2",
        (char *)"player3",
        (char *)"player4",
        (char *)"--mode=fast"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(ArgsParserTest, TestParseArgumentsValidTournamentMode)
{
    // Подготовка аргументов с более чем 3 игроками в турнирном режиме
    char *argv[] = {
        (char *)"program_name",
        (char *)"player1",
        (char *)"player2",
        (char *)"player3",
        (char *)"player4",
        (char *)"--mode=tournament"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    auto args = parse_arguments(argc, argv);
    EXPECT_EQ(args->players.size(), 4);
}

// Тесты для функции load_matrix
TEST(ArgsParserTest, TestLoadMatrixValidFile)
{

    std::ofstream file("matrix_test.txt");
    file << "77 77 77\n33 33 99\n33 99 33\n0 55 55\n99 33 33\n55 0 55\n55 55 0\n11 11 11" << std::endl;
    file.close();
    auto matrix = load_matrix("matrix_test.txt");

    EXPECT_EQ(matrix[0][0], 77);
    EXPECT_EQ(matrix[1][1], 33);
    EXPECT_EQ(matrix[2][2], 33);
    EXPECT_EQ(matrix[3][2], 55);
    EXPECT_EQ(matrix[7][1], 11);

    std::remove("matrix_test.txt");
}

TEST(ArgsParserTest, TestLoadMatrixFileNotFound)
{
    EXPECT_THROW(load_matrix("nonexistent_file.txt"), std::runtime_error);
}

TEST(ArgsParserTest, TestLoadMatrixInvalidData)
{
    // Создаем файл с некорректной матрицей
    std::ofstream file("matrix_test.txt");
    file << "7 7 7\n3 3 9\nHOHOHO Y O\n0 5 5\n9 3 3\n5 0 5\nAWWW\n1 1 1\n"
         << std::endl;
    file.close();

    EXPECT_THROW(load_matrix("matrix_test.txt"), std::runtime_error);

    std::remove("matrix_test.txt");
}

TEST(ArgsParserTest, TestParseArgumentsZeroInput)
{
    char *argv[] = {
        (char *)"program_name",

    };

    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
