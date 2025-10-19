#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <map>
#include <vector>
#include "Order.h"

// Structure to hold position information
struct Position {
    std::string symbol;
    int quantity;
    double averagePrice;
    double currentValue;
    double profitLoss;
    
    Position() : quantity(0), averagePrice(0.0), currentValue(0.0), profitLoss(0.0) {}
    Position(const std::string& sym, int qty, double avgPrice)
        : symbol(sym), quantity(qty), averagePrice(avgPrice), 
          currentValue(0.0), profitLoss(0.0) {}
};

// Transaction record for history
struct Transaction {
    std::string type; // "BUY" or "SELL"
    std::string symbol;
    int quantity;
    double price;
    time_t timestamp;
    
    Transaction(const std::string& t, const std::string& s, int q, double p)
        : type(t), symbol(s), quantity(q), price(p), timestamp(std::time(nullptr)) {}
    
    std::string serialize() const;
    static Transaction deserialize(const std::string& data);
};

// Portfolio class demonstrating Composition (has-a relationships)
class Portfolio {
private:
    std::string userId;
    double cashBalance;
    std::map<std::string, Position> positions; // symbol -> Position
    std::vector<Transaction> transactionHistory;

public:
    // Constructor
    Portfolio(const std::string& userId, double initialBalance = 100000.0);
    
    // Getters
    std::string getUserId() const;
    double getCashBalance() const;
    const std::map<std::string, Position>& getPositions() const;
    const std::vector<Transaction>& getTransactionHistory() const;
    
    // Portfolio operations
    bool buyStock(const std::string& symbol, int quantity, double price);
    bool sellStock(const std::string& symbol, int quantity, double price);
    void updatePositionValues(const std::map<std::string, double>& currentPrices);
    
    // Portfolio metrics
    double getTotalValue(const std::map<std::string, double>& currentPrices) const;
    double getTotalProfitLoss() const;
    Position* getPosition(const std::string& symbol);
    
    // Display
    void displayPositions() const;
    void displayTransactionHistory(int limit = 10) const;
    
    // Serialization
    std::string serialize() const;
    static Portfolio deserialize(const std::string& data);
};

#endif
