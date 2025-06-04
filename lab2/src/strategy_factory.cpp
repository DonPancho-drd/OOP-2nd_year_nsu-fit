#include <vector>
#include <stdexcept>

#include "strategy_factory.h"

std::shared_ptr<Strategy> StrategyFactory::createStrategy(std::string strategy_name)
{
    if (strategy_name == "poor_trusting_fool")
    {
        return std::make_shared<PoorTrustingFool>();
    }
    else if (strategy_name == "tit_for_tat")
    {
        return std::make_shared<TitForTat>();
    }
    else if (strategy_name == "all_defector")
    {
        return std::make_shared<AllDefector>();
    }
    else if (strategy_name == "go_by_majority")
    {
        return std::make_shared<GoByMajority>();
    }
    else if (strategy_name == "random")
    {
        return std::make_shared<Random>();
    }
    else if (strategy_name == "periodic")
    {
        return std::make_shared<Periodic>();
    }
    else
    {
        throw std::invalid_argument("Invalid strategy name");
    }
}
