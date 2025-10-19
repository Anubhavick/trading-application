#include "StrategyEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

// Base TradingStrategy implementation
TradingStrategy::TradingStrategy(const std::string& name, const std::string& desc)
    : strategyName(name), description(desc) {}

std::string TradingStrategy::getStrategyName() const {
    return strategyName;
}

std::string TradingStrategy::getDescription() const {
    return description;
}

void TradingStrategy::displayInfo() const {
    std::cout << "Strategy: " << strategyName << std::endl;
    std::cout << "Description: " << description << std::endl;
}

// BuyBelowPriceStrategy implementation
BuyBelowPriceStrategy::BuyBelowPriceStrategy(double threshold, int qty)
    : TradingStrategy("Buy Below Price", 
                     "Buy stocks when price falls below threshold"),
      priceThreshold(threshold), quantity(qty) {}

std::vector<std::unique_ptr<Order>> BuyBelowPriceStrategy::generateSignals(
    const std::map<std::string, Stock>& stocks,
    const Portfolio& /* portfolio */) {
    
    std::vector<std::unique_ptr<Order>> signals;
    
    for (const auto& pair : stocks) {
        const Stock& stock = pair.second;
        
        if (stock.getCurrentPrice() < priceThreshold) {
            // Generate buy signal
            signals.push_back(std::unique_ptr<Order>(new BuyOrder(
                stock.getSymbol(), quantity, stock.getCurrentPrice())));
            
            std::cout << "Signal: BUY " << stock.getSymbol() 
                      << " (Price: $" << std::fixed << std::setprecision(2)
                      << stock.getCurrentPrice() << " < Threshold: $" 
                      << priceThreshold << ")" << std::endl;
        }
    }
    
    if (signals.empty()) {
        std::cout << "No buy signals generated. No stocks below $" 
                  << priceThreshold << std::endl;
    }
    
    return signals;
}

void BuyBelowPriceStrategy::displayInfo() const {
    TradingStrategy::displayInfo();
    std::cout << "Price Threshold: $" << std::fixed << std::setprecision(2) 
              << priceThreshold << std::endl;
    std::cout << "Quantity per trade: " << quantity << std::endl;
}

// MovingAverageCrossoverStrategy implementation
MovingAverageCrossoverStrategy::MovingAverageCrossoverStrategy(int shortMA, int longMA, int qty)
    : TradingStrategy("MA Crossover", 
                     "Buy when short MA crosses above long MA, sell when below"),
      shortPeriod(shortMA), longPeriod(longMA), quantity(qty) {}

std::vector<std::unique_ptr<Order>> MovingAverageCrossoverStrategy::generateSignals(
    const std::map<std::string, Stock>& stocks,
    const Portfolio& portfolio) {
    
    std::vector<std::unique_ptr<Order>> signals;
    
    for (const auto& pair : stocks) {
        const Stock& stock = pair.second;
        
        // Need enough price history
        if (stock.getPriceHistory().size() < static_cast<size_t>(longPeriod)) {
            continue;
        }
        
        double shortMA = stock.getMovingAverage(shortPeriod);
        double longMA = stock.getMovingAverage(longPeriod);
        
        // Buy signal: short MA > long MA and we don't own the stock
        if (shortMA > longMA && portfolio.getPositions().find(stock.getSymbol()) == 
            portfolio.getPositions().end()) {
            
            signals.push_back(std::unique_ptr<Order>(new BuyOrder(
                stock.getSymbol(), quantity, stock.getCurrentPrice())));
            
            std::cout << "Signal: BUY " << stock.getSymbol() 
                      << " (Short MA: $" << std::fixed << std::setprecision(2) << shortMA
                      << " > Long MA: $" << longMA << ")" << std::endl;
        }
        // Sell signal: short MA < long MA and we own the stock
        else if (shortMA < longMA && portfolio.getPositions().find(stock.getSymbol()) != 
                 portfolio.getPositions().end()) {
            
            const Position& pos = portfolio.getPositions().at(stock.getSymbol());
            
            signals.push_back(std::unique_ptr<Order>(new SellOrder(
                stock.getSymbol(), pos.quantity, stock.getCurrentPrice())));
            
            std::cout << "Signal: SELL " << stock.getSymbol() 
                      << " (Short MA: $" << std::fixed << std::setprecision(2) << shortMA
                      << " < Long MA: $" << longMA << ")" << std::endl;
        }
    }
    
    if (signals.empty()) {
        std::cout << "No MA crossover signals generated." << std::endl;
    }
    
    return signals;
}

void MovingAverageCrossoverStrategy::displayInfo() const {
    TradingStrategy::displayInfo();
    std::cout << "Short MA Period: " << shortPeriod << std::endl;
    std::cout << "Long MA Period: " << longPeriod << std::endl;
    std::cout << "Quantity per trade: " << quantity << std::endl;
}

// MeanReversionStrategy implementation
MeanReversionStrategy::MeanReversionStrategy(int period, double threshold, int qty)
    : TradingStrategy("Mean Reversion", 
                     "Buy when price deviates below mean, sell when above"),
      period(period), deviationThreshold(threshold), quantity(qty) {}

std::vector<std::unique_ptr<Order>> MeanReversionStrategy::generateSignals(
    const std::map<std::string, Stock>& stocks,
    const Portfolio& portfolio) {
    
    std::vector<std::unique_ptr<Order>> signals;
    
    for (const auto& pair : stocks) {
        const Stock& stock = pair.second;
        
        // Need enough price history
        if (stock.getPriceHistory().size() < static_cast<size_t>(period)) {
            continue;
        }
        
        double mean = stock.getMovingAverage(period);
        double currentPrice = stock.getCurrentPrice();
        double deviation = (currentPrice - mean) / mean;
        
        // Buy signal: price significantly below mean
        if (deviation < -deviationThreshold && 
            portfolio.getPositions().find(stock.getSymbol()) == 
            portfolio.getPositions().end()) {
            
            signals.push_back(std::unique_ptr<Order>(new BuyOrder(
                stock.getSymbol(), quantity, currentPrice)));
            
            std::cout << "Signal: BUY " << stock.getSymbol() 
                      << " (Price: $" << std::fixed << std::setprecision(2) << currentPrice
                      << " is " << (deviation * 100) << "% below mean: $" 
                      << mean << ")" << std::endl;
        }
        // Sell signal: price significantly above mean
        else if (deviation > deviationThreshold && 
                 portfolio.getPositions().find(stock.getSymbol()) != 
                 portfolio.getPositions().end()) {
            
            const Position& pos = portfolio.getPositions().at(stock.getSymbol());
            
            signals.push_back(std::unique_ptr<Order>(new SellOrder(
                stock.getSymbol(), pos.quantity, currentPrice)));
            
            std::cout << "Signal: SELL " << stock.getSymbol() 
                      << " (Price: $" << std::fixed << std::setprecision(2) << currentPrice
                      << " is " << (deviation * 100) << "% above mean: $" 
                      << mean << ")" << std::endl;
        }
    }
    
    if (signals.empty()) {
        std::cout << "No mean reversion signals generated." << std::endl;
    }
    
    return signals;
}

void MeanReversionStrategy::displayInfo() const {
    TradingStrategy::displayInfo();
    std::cout << "Period: " << period << std::endl;
    std::cout << "Deviation Threshold: " << (deviationThreshold * 100) << "%" << std::endl;
    std::cout << "Quantity per trade: " << quantity << std::endl;
}

// StrategyEngine implementation
StrategyEngine::StrategyEngine() {
    // Initialize with default strategies
    strategies.push_back(std::unique_ptr<TradingStrategy>(new BuyBelowPriceStrategy(200.0, 5)));
    strategies.push_back(std::unique_ptr<TradingStrategy>(new MovingAverageCrossoverStrategy(5, 20, 10)));
    strategies.push_back(std::unique_ptr<TradingStrategy>(new MeanReversionStrategy(20, 0.05, 8)));
}

void StrategyEngine::addStrategy(std::unique_ptr<TradingStrategy> strategy) {
    strategies.push_back(std::move(strategy));
}

const std::vector<std::unique_ptr<TradingStrategy>>& StrategyEngine::getStrategies() const {
    return strategies;
}

std::vector<std::unique_ptr<Order>> StrategyEngine::runStrategy(
    int strategyIndex,
    const std::map<std::string, Stock>& stocks,
    const Portfolio& portfolio) {
    
    if (strategyIndex < 0 || static_cast<size_t>(strategyIndex) >= strategies.size()) {
        std::cout << "Invalid strategy index." << std::endl;
        return std::vector<std::unique_ptr<Order>>();
    }
    
    std::cout << "\n=== Running Strategy: " 
              << strategies[strategyIndex]->getStrategyName() << " ===" << std::endl;
    
    return strategies[strategyIndex]->generateSignals(stocks, portfolio);
}

void StrategyEngine::displayStrategies() const {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "AVAILABLE TRADING STRATEGIES" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    for (size_t i = 0; i < strategies.size(); i++) {
        std::cout << "\n[" << (i + 1) << "] ";
        strategies[i]->displayInfo();
        std::cout << std::string(70, '-') << std::endl;
    }
}
