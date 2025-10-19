#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include "../models/Stock.h"
#include "../models/Portfolio.h"
#include "../models/Order.h"

// Abstract Strategy class demonstrating Abstraction and Polymorphism
class TradingStrategy {
protected:
    std::string strategyName;
    std::string description;

public:
    TradingStrategy(const std::string& name, const std::string& desc);
    virtual ~TradingStrategy() = default;
    
    // Pure virtual function - must be implemented by derived classes
    virtual std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) = 0;
    
    // Getters
    std::string getStrategyName() const;
    std::string getDescription() const;
    
    // Display
    virtual void displayInfo() const;
};

// Concrete strategy: Buy when price is below a threshold
class BuyBelowPriceStrategy : public TradingStrategy {
private:
    double priceThreshold;
    int quantity;

public:
    BuyBelowPriceStrategy(double threshold, int qty = 10);
    
    std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) override;
    
    void displayInfo() const override;
};

// Concrete strategy: Moving Average Crossover
class MovingAverageCrossoverStrategy : public TradingStrategy {
private:
    int shortPeriod;
    int longPeriod;
    int quantity;

public:
    MovingAverageCrossoverStrategy(int shortMA = 5, int longMA = 20, int qty = 10);
    
    std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) override;
    
    void displayInfo() const override;
};

// Concrete strategy: Mean Reversion
class MeanReversionStrategy : public TradingStrategy {
private:
    int period;
    double deviationThreshold;
    int quantity;

public:
    MeanReversionStrategy(int period = 20, double threshold = 0.05, int qty = 10);
    
    std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) override;
    
    void displayInfo() const override;
};

// Strategy Engine manages and executes strategies
class StrategyEngine {
private:
    std::vector<std::unique_ptr<TradingStrategy>> strategies;

public:
    StrategyEngine();
    
    // Strategy management
    void addStrategy(std::unique_ptr<TradingStrategy> strategy);
    const std::vector<std::unique_ptr<TradingStrategy>>& getStrategies() const;
    
    // Execute a strategy
    std::vector<std::unique_ptr<Order>> runStrategy(
        int strategyIndex,
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio);
    
    // Display available strategies
    void displayStrategies() const;
};

#endif
