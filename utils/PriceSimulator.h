#ifndef PRICE_SIMULATOR_H
#define PRICE_SIMULATOR_H

#include <random>
#include <map>
#include "../models/Stock.h"

// PriceSimulator simulates realistic stock price movements
class PriceSimulator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<double> distribution;
    
    double volatility;  // Standard deviation of price changes
    double drift;       // Average price drift (positive = upward trend)

public:
    // Constructor
    PriceSimulator(double volatility = 0.02, double drift = 0.0001);
    
    // Simulate price change for a single stock
    void simulatePriceChange(Stock& stock);
    
    // Simulate price changes for all stocks
    void simulateMarket(std::map<std::string, Stock>& stocks);
    
    // Setters
    void setVolatility(double vol);
    void setDrift(double d);
    
    // Simulate specific scenarios
    void simulateBullMarket(std::map<std::string, Stock>& stocks);
    void simulateBearMarket(std::map<std::string, Stock>& stocks);
    void simulateVolatileMarket(std::map<std::string, Stock>& stocks);
};

#endif
