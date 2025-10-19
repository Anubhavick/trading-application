#include "PriceSimulator.h"
#include <iostream>
#include <iomanip>

PriceSimulator::PriceSimulator(double volatility, double drift)
    : gen(rd()), distribution(drift, volatility), 
      volatility(volatility), drift(drift) {}

void PriceSimulator::simulatePriceChange(Stock& stock) {
    double currentPrice = stock.getCurrentPrice();
    
    // Generate random price change percentage
    double changePercent = distribution(gen);
    
    // Calculate new price
    double newPrice = currentPrice * (1.0 + changePercent);
    
    // Ensure price doesn't go below a minimum threshold
    if (newPrice < 1.0) {
        newPrice = 1.0;
    }
    
    stock.setCurrentPrice(newPrice);
}

void PriceSimulator::simulateMarket(std::map<std::string, Stock>& stocks) {
    std::cout << "\n=== Simulating Market Price Changes ===" << std::endl;
    
    for (auto& pair : stocks) {
        Stock& stock = pair.second;
        double oldPrice = stock.getCurrentPrice();
        
        simulatePriceChange(stock);
        
        double newPrice = stock.getCurrentPrice();
        double change = newPrice - oldPrice;
        double changePercent = (change / oldPrice) * 100.0;
        
        std::cout << stock.getSymbol() << ": $" 
                  << std::fixed << std::setprecision(2) << oldPrice
                  << " -> $" << newPrice
                  << " (" << std::showpos << changePercent << std::noshowpos << "%)" 
                  << std::endl;
    }
    
    std::cout << "Market update complete." << std::endl;
}

void PriceSimulator::setVolatility(double vol) {
    volatility = vol;
    distribution = std::normal_distribution<double>(drift, volatility);
}

void PriceSimulator::setDrift(double d) {
    drift = d;
    distribution = std::normal_distribution<double>(drift, volatility);
}

void PriceSimulator::simulateBullMarket(std::map<std::string, Stock>& stocks) {
    std::cout << "\n=== Simulating BULL MARKET (Upward Trend) ===" << std::endl;
    
    // Temporarily increase drift for positive trend
    double originalDrift = drift;
    setDrift(0.03); // 3% average increase
    
    simulateMarket(stocks);
    
    // Restore original drift
    setDrift(originalDrift);
}

void PriceSimulator::simulateBearMarket(std::map<std::string, Stock>& stocks) {
    std::cout << "\n=== Simulating BEAR MARKET (Downward Trend) ===" << std::endl;
    
    // Temporarily decrease drift for negative trend
    double originalDrift = drift;
    setDrift(-0.03); // 3% average decrease
    
    simulateMarket(stocks);
    
    // Restore original drift
    setDrift(originalDrift);
}

void PriceSimulator::simulateVolatileMarket(std::map<std::string, Stock>& stocks) {
    std::cout << "\n=== Simulating VOLATILE MARKET (High Fluctuation) ===" << std::endl;
    
    // Temporarily increase volatility
    double originalVolatility = volatility;
    setVolatility(0.05); // 5% standard deviation
    
    simulateMarket(stocks);
    
    // Restore original volatility
    setVolatility(originalVolatility);
}
