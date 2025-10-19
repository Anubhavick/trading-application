#include "Stock.h"
#include "../utils/Colors.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

Stock::Stock() : symbol(""), name(""), currentPrice(0.0), lastUpdate(std::time(nullptr)) {
}

Stock::Stock(const std::string& symbol, const std::string& name, double initialPrice)
    : symbol(symbol), name(name), currentPrice(initialPrice), lastUpdate(std::time(nullptr)) {
    priceHistory.push_back(initialPrice);
}

std::string Stock::getSymbol() const {
    return symbol;
}

std::string Stock::getName() const {
    return name;
}

double Stock::getCurrentPrice() const {
    return currentPrice;
}

const std::vector<double>& Stock::getPriceHistory() const {
    return priceHistory;
}

time_t Stock::getLastUpdate() const {
    return lastUpdate;
}

void Stock::setCurrentPrice(double price) {
    currentPrice = price;
    lastUpdate = std::time(nullptr);
    addPriceToHistory(price);
}

void Stock::addPriceToHistory(double price) {
    priceHistory.push_back(price);
    // Keep only last 100 prices to save memory
    if (priceHistory.size() > 100) {
        priceHistory.erase(priceHistory.begin());
    }
}

double Stock::getMovingAverage(int period) const {
    if (priceHistory.empty()) return 0.0;
    
    int count = std::min(period, static_cast<int>(priceHistory.size()));
    double sum = 0.0;
    
    for (size_t i = priceHistory.size() - count; i < priceHistory.size(); i++) {
        sum += priceHistory[i];
    }
    
    return sum / count;
}

double Stock::getPriceChange() const {
    if (priceHistory.size() < 2) return 0.0;
    return currentPrice - priceHistory[priceHistory.size() - 2];
}

double Stock::getPriceChangePercent() const {
    if (priceHistory.size() < 2 || priceHistory[priceHistory.size() - 2] == 0) return 0.0;
    return (getPriceChange() / priceHistory[priceHistory.size() - 2]) * 100.0;
}

void Stock::display() const {
    double change = getPriceChange();
    double changePercent = getPriceChangePercent();
    
    std::cout << Colors::BOLD_CYAN << std::left << std::setw(8) << symbol << Colors::RESET
              << std::setw(20) << name
              << Colors::BOLD_WHITE << std::right << std::setw(10) << std::fixed << std::setprecision(2) << currentPrice << Colors::RESET;
    
    // Color code the change
    if (change > 0) {
        std::cout << Colors::PROFIT << std::setw(12) << Symbols::ARROW_UP << " " << change
                  << std::setw(9) << changePercent << "%" << Colors::RESET;
    } else if (change < 0) {
        std::cout << Colors::LOSS << std::setw(12) << Symbols::ARROW_DOWN << " " << change
                  << std::setw(9) << changePercent << "%" << Colors::RESET;
    } else {
        std::cout << Colors::DIM << std::setw(12) << "  " << change
                  << std::setw(9) << changePercent << "%" << Colors::RESET;
    }
}

std::string Stock::serialize() const {
    std::ostringstream oss;
    oss << symbol << "|" << name << "|" << currentPrice << "|";
    
    // Serialize price history
    for (size_t i = 0; i < priceHistory.size(); i++) {
        oss << priceHistory[i];
        if (i < priceHistory.size() - 1) oss << ",";
    }
    
    return oss.str();
}

Stock Stock::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string symbol, name, priceStr, historyStr;
    
    std::getline(iss, symbol, '|');
    std::getline(iss, name, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, historyStr);
    
    double price = std::stod(priceStr);
    Stock stock(symbol, name, price);
    
    // Deserialize price history
    stock.priceHistory.clear();
    std::istringstream historyStream(historyStr);
    std::string priceItem;
    
    while (std::getline(historyStream, priceItem, ',')) {
        if (!priceItem.empty()) {
            stock.priceHistory.push_back(std::stod(priceItem));
        }
    }
    
    return stock;
}
