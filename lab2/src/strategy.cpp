#include "strategy.h"

// --------PoorTrustingFool--------

Move PoorTrustingFool::makeMove() {
    return Move::COOPERATE;
}

void PoorTrustingFool::applyExperience(const std::pair<Move, Move> &decisions) {}


// --------AllDefector--------

Move AllDefector::makeMove() {
    return Move::DEFECT;
}

void AllDefector::applyExperience(const std::pair<Move, Move> &decisions) {}

// --------TitForTat--------

Move TitForTat::makeMove() {
    return last_move;
}

void TitForTat::applyExperience(const std::pair<Move, Move> &decisions) {
    last_move = decisions.first;
}


// --------GoByMajority--------

Move GoByMajority::makeMove() {

    return c_cnt >= d_cnt ? Move::COOPERATE : Move::DEFECT;  // первый ход - cooperate
}

void GoByMajority::applyExperience(const std::pair<Move, Move> &decisions) {

    if (decisions.first == Move::COOPERATE) {
        c_cnt++;
    } else {
        d_cnt++;
    }

    if (decisions.second == Move::COOPERATE) {
        c_cnt++;
    } else {
        d_cnt++;
    }
}

// --------Random--------

Move Random::makeMove(){
    srand(time(0));
    return Move(rand() % 2 == 0 ? Move::COOPERATE : Move::DEFECT);
}

void Random::applyExperience(const std::pair<Move, Move> &decisions) {}


// --------Periodic--------

Move Periodic::makeMove(){
    if(last_move == Move::COOPERATE){
        return Move::DEFECT;
    } else{
        return Move::COOPERATE;
    }
}

void Periodic::applyExperience(const std::pair<Move, Move> &decisions){
    last_move = (last_move == Move::COOPERATE) ? Move::DEFECT : Move::COOPERATE;
}
