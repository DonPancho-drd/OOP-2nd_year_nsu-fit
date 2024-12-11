#pragma once

#include <string>
#include <memory>

#include "strategy.h"

class StrategyFactory {
    StrategyFactory() = delete;
public:
    static std::shared_ptr<Strategy> createStrategy(std::string &strategy_name);
};
