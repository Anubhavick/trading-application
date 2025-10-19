#include "TradingEngine.h"
#include "../utils/Colors.h"
#include <iostream>
#include <iomanip>
#include <sstream>

TradingEngine::TradingEngine() {
    // Initialize with some default stocks
    addStock(Stock("AAPL", "Apple Inc.", 150.50));
    addStock(Stock("GOOGL", "Alphabet Inc.", 2800.75));
    addStock(Stock("MSFT", "Microsoft Corp.", 310.25));
    addStock(Stock("TSLA", "Tesla Inc.", 245.80));
    addStock(Stock("AMZN", "Amazon.com Inc.", 135.40));
}

void TradingEngine::addStock(const Stock& stock) {
    stocks[stock.getSymbol()] = stock;
    std::cout << Colors::SUCCESS << Symbols::CHECK << " Stock " << stock.getSymbol() << " added successfully." << Colors::RESET << std::endl;
}

bool TradingEngine::removeStock(const std::string& symbol) {
    auto it = stocks.find(symbol);
    if (it != stocks.end()) {
        stocks.erase(it);
        std::cout << Colors::WARNING << Symbols::CHECK << " Stock " << symbol << " removed successfully." << Colors::RESET << std::endl;
        return true;
    }
    std::cout << Colors::ERROR << Symbols::CROSS << " Stock " << symbol << " not found." << Colors::RESET << std::endl;
    return false;
}

Stock* TradingEngine::getStock(const std::string& symbol) {
    auto it = stocks.find(symbol);
    if (it != stocks.end()) {
        return &(it->second);
    }
    return nullptr;
}

const std::map<std::string, Stock>& TradingEngine::getAllStocks() const {
    return stocks;
}

bool TradingEngine::stockExists(const std::string& symbol) const {
    return stocks.find(symbol) != stocks.end();
}

bool TradingEngine::executeOrder(Order* order, Portfolio& portfolio) {
    // Polymorphic behavior - determine order type at runtime
    if (BuyOrder* buyOrder = dynamic_cast<BuyOrder*>(order)) {
        return executeBuyOrder(buyOrder, portfolio);
    } else if (SellOrder* sellOrder = dynamic_cast<SellOrder*>(order)) {
        return executeSellOrder(sellOrder, portfolio);
    }
    return false;
}

bool TradingEngine::executeBuyOrder(BuyOrder* order, Portfolio& portfolio) {
    if (!stockExists(order->getSymbol())) {
        std::cout << "Stock " << order->getSymbol() << " does not exist." << std::endl;
        order->setStatus("CANCELLED");
        return false;
    }
    
    Stock* stock = getStock(order->getSymbol());
    double currentPrice = stock->getCurrentPrice();
    
    // Execute at current market price
    bool success = portfolio.buyStock(order->getSymbol(), order->getQuantity(), currentPrice);
    
    if (success) {
        order->setStatus("EXECUTED");
        std::cout << Colors::SUCCESS << Symbols::CHECK << " Buy order executed: " << order->getQuantity() << " shares of " 
                  << order->getSymbol() << " at " << Colors::BOLD_WHITE << "$" << std::fixed << std::setprecision(2) 
                  << currentPrice << Colors::RESET << std::endl;
        return true;
    } else {
        order->setStatus("CANCELLED");
        return false;
    }
}

bool TradingEngine::executeSellOrder(SellOrder* order, Portfolio& portfolio) {
    if (!stockExists(order->getSymbol())) {
        std::cout << "Stock " << order->getSymbol() << " does not exist." << std::endl;
        order->setStatus("CANCELLED");
        return false;
    }
    
    Stock* stock = getStock(order->getSymbol());
    double currentPrice = stock->getCurrentPrice();
    
    // Execute at current market price
    bool success = portfolio.sellStock(order->getSymbol(), order->getQuantity(), currentPrice);
    
    if (success) {
        order->setStatus("EXECUTED");
        std::cout << Colors::SUCCESS << Symbols::CHECK << " Sell order executed: " << order->getQuantity() << " shares of " 
                  << order->getSymbol() << " at " << Colors::BOLD_WHITE << "$" << std::fixed << std::setprecision(2) 
                  << currentPrice << Colors::RESET << std::endl;
        return true;
    } else {
        order->setStatus("CANCELLED");
        return false;
    }
}

void TradingEngine::displayMarket() const {
    if (stocks.empty()) {
        std::cout << Colors::WARNING << "No stocks in the market." << Colors::RESET << std::endl;
        return;
    }
    
    std::cout << "\n" << Colors::HEADER << std::string(80, '=') << Colors::RESET << std::endl;
    std::cout << Colors::BOLD_CYAN << "STOCK MARKET" << Colors::RESET << std::endl;
    std::cout << Colors::HEADER << std::string(80, '=') << Colors::RESET << std::endl;
    
    std::cout << Colors::BOLD << std::left << std::setw(8) << "Symbol"
              << std::setw(20) << "Name"
              << std::right << std::setw(10) << "Price"
              << std::setw(12) << "Change"
              << std::setw(10) << "Change%" << Colors::RESET << std::endl;
    std::cout << Colors::DIM << std::string(80, '-') << Colors::RESET << std::endl;
    
    for (const auto& pair : stocks) {
        pair.second.display();
        std::cout << std::endl;
    }
    
    std::cout << Colors::HEADER << std::string(80, '=') << Colors::RESET << std::endl;
}

void TradingEngine::displayStockDetails(const std::string& symbol) const {
    auto it = stocks.find(symbol);
    if (it == stocks.end()) {
        std::cout << "Stock " << symbol << " not found." << std::endl;
        return;
    }
    
    const Stock& stock = it->second;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "STOCK DETAILS: " << symbol << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Name: " << stock.getName() << std::endl;
    std::cout << "Current Price: $" << std::fixed << std::setprecision(2) 
              << stock.getCurrentPrice() << std::endl;
    std::cout << "Price Change: $" << stock.getPriceChange() 
              << " (" << stock.getPriceChangePercent() << "%)" << std::endl;
    std::cout << "5-Period MA: $" << stock.getMovingAverage(5) << std::endl;
    std::cout << "10-Period MA: $" << stock.getMovingAverage(10) << std::endl;
    std::cout << "20-Period MA: $" << stock.getMovingAverage(20) << std::endl;
    
    // Display recent price history
    const auto& history = stock.getPriceHistory();
    int count = std::min(10, static_cast<int>(history.size()));
    
    std::cout << "\nRecent Price History (Last " << count << "):" << std::endl;
    for (size_t i = history.size() - count; i < history.size(); i++) {
        std::cout << "  " << std::fixed << std::setprecision(2) << history[i] << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

std::map<std::string, double> TradingEngine::getCurrentPrices() const {
    std::map<std::string, double> prices;
    for (const auto& pair : stocks) {
        prices[pair.first] = pair.second.getCurrentPrice();
    }
    return prices;
}

std::string TradingEngine::serializeStocks() const {
    std::ostringstream oss;
    oss << stocks.size() << "\n";
    
    for (const auto& pair : stocks) {
        oss << pair.second.serialize() << "\n";
    }
    
    return oss.str();
}

void TradingEngine::deserializeStocks(const std::string& data) {
    stocks.clear();
    
    std::istringstream iss(data);
    std::string line;
    
    // Read count
    if (!std::getline(iss, line)) return;
    int count = std::stoi(line);
    
    // Read stocks
    for (int i = 0; i < count; i++) {
        if (!std::getline(iss, line)) break;
        Stock stock = Stock::deserialize(line);
        stocks[stock.getSymbol()] = stock;
    }
}
