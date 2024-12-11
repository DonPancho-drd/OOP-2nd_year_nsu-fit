#pragma once

#include <string>
#include <memory>

enum class Move{COOPERATE, DEFECT};

class Strategy{
    protected:
    std::string strategy_name;
public:
    virtual ~Strategy() = default;
    std::string getStrategyName() const { return strategy_name; };

    virtual Move makeMove() = 0;
    virtual void applyExperience(const std::pair<Move, Move> &decisions) = 0;


};

class PoorTrustingFool : public Strategy{
    public:
    PoorTrustingFool() {strategy_name = "poor_trusting_fool";};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};

class AllDefector : public Strategy{
    public:
    AllDefector() {strategy_name = "all_defector";};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};

class TitForTat : public Strategy{
    Move last_move;
public:
    TitForTat() {strategy_name = "tit_for_tat";
                last_move = Move::COOPERATE;};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};


class GoByMajority : public Strategy{
    int c_cnt;
    int d_cnt;
public:
    GoByMajority() : c_cnt(0), d_cnt(0)
    {strategy_name = "go_by_majority";};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};


class Random : public Strategy{
public:
    Random() {strategy_name = "random";};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};


class Periodic : public Strategy{
    Move last_move = Move::COOPERATE;
public:
    Periodic() {strategy_name = "periodic";};

    Move makeMove() override;
    void applyExperience(const std::pair<Move, Move> &decisions) override;
};
