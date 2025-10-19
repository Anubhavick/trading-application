#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>
#include <ctime>

class Stock {
private:
    std::string symbol;
    std::string name;
    double currentPrice;
    std::vector<double> priceHistory;
    time_t lastUpdate;

public:
    // Constructors
    Stock(); // Default constructor for std::map
    Stock(const std::string& symbol, const std::string& name, double initialPrice);
    
    // Getters (Encapsulation)
    std::string getSymbol() const;
    std::string getName() const;
    double getCurrentPrice() const;
    const std::vector<double>& getPriceHistory() const;
    time_t getLastUpdate() const;
    
    // Setters
    void setCurrentPrice(double price);
    void addPriceToHistory(double price);
    
    // Business methods
    double getMovingAverage(int period) const;
    double getPriceChange() const;
    double getPriceChangePercent() const;
    
    // Display
    void display() const;
    
    // Serialization
    std::string serialize() const;
    static Stock deserialize(const std::string& data);
};

#endif
