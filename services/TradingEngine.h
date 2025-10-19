#ifndef TRADING_ENGINE_H
#define TRADING_ENGINE_H

#include <vector>
#include <map>
#include <memory>
#include "../models/Stock.h"
#include "../models/Order.h"
#include "../models/Portfolio.h"

// TradingEngine manages stocks and executes orders
class TradingEngine {
private:
    std::map<std::string, Stock> stocks; // symbol -> Stock
    std::vector<std::unique_ptr<Order>> orderBook;

public:
    // Constructor
    TradingEngine();
    
    // Stock management
    void addStock(const Stock& stock);
    bool removeStock(const std::string& symbol);
    Stock* getStock(const std::string& symbol);
    const std::map<std::string, Stock>& getAllStocks() const;
    bool stockExists(const std::string& symbol) const;
    
    // Order execution using polymorphism
    bool executeOrder(Order* order, Portfolio& portfolio);
    bool executeBuyOrder(BuyOrder* order, Portfolio& portfolio);
    bool executeSellOrder(SellOrder* order, Portfolio& portfolio);
    
    // Market display
    void displayMarket() const;
    void displayStockDetails(const std::string& symbol) const;
    
    // Price updates
    std::map<std::string, double> getCurrentPrices() const;
    
    // Serialization
    std::string serializeStocks() const;
    void deserializeStocks(const std::string& data);
};

#endif
