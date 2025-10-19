#include "Portfolio.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

// Transaction implementation
std::string Transaction::serialize() const {
    std::ostringstream oss;
    oss << type << "|" << symbol << "|" << quantity << "|" 
        << price << "|" << timestamp;
    return oss.str();
}

Transaction Transaction::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type, symbol, qtyStr, priceStr, timeStr;
    
    std::getline(iss, type, '|');
    std::getline(iss, symbol, '|');
    std::getline(iss, qtyStr, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, timeStr);
    
    Transaction txn(type, symbol, std::stoi(qtyStr), std::stod(priceStr));
    txn.timestamp = std::stol(timeStr);
    
    return txn;
}

// Portfolio implementation
Portfolio::Portfolio(const std::string& userId, double initialBalance)
    : userId(userId), cashBalance(initialBalance) {}

std::string Portfolio::getUserId() const {
    return userId;
}

double Portfolio::getCashBalance() const {
    return cashBalance;
}

const std::map<std::string, Position>& Portfolio::getPositions() const {
    return positions;
}

const std::vector<Transaction>& Portfolio::getTransactionHistory() const {
    return transactionHistory;
}

bool Portfolio::buyStock(const std::string& symbol, int quantity, double price) {
    double totalCost = quantity * price;
    
    if (totalCost > cashBalance) {
        std::cout << "Insufficient funds. Required: $" << std::fixed 
                  << std::setprecision(2) << totalCost 
                  << ", Available: $" << cashBalance << std::endl;
        return false;
    }
    
    // Deduct cash
    cashBalance -= totalCost;
    
    // Update or create position
    if (positions.find(symbol) != positions.end()) {
        Position& pos = positions[symbol];
        double totalCost = (pos.quantity * pos.averagePrice) + (quantity * price);
        pos.quantity += quantity;
        pos.averagePrice = totalCost / pos.quantity;
    } else {
        positions[symbol] = Position(symbol, quantity, price);
    }
    
    // Record transaction
    transactionHistory.push_back(Transaction("BUY", symbol, quantity, price));
    
    return true;
}

bool Portfolio::sellStock(const std::string& symbol, int quantity, double price) {
    if (positions.find(symbol) == positions.end()) {
        std::cout << "You don't own any shares of " << symbol << std::endl;
        return false;
    }
    
    Position& pos = positions[symbol];
    
    if (pos.quantity < quantity) {
        std::cout << "Insufficient shares. You own " << pos.quantity 
                  << " shares of " << symbol << std::endl;
        return false;
    }
    
    // Credit cash
    double totalValue = quantity * price;
    cashBalance += totalValue;
    
    // Update position
    pos.quantity -= quantity;
    
    // Remove position if quantity is zero
    if (pos.quantity == 0) {
        positions.erase(symbol);
    }
    
    // Record transaction
    transactionHistory.push_back(Transaction("SELL", symbol, quantity, price));
    
    return true;
}

void Portfolio::updatePositionValues(const std::map<std::string, double>& currentPrices) {
    for (auto& pair : positions) {
        Position& pos = pair.second;
        
        if (currentPrices.find(pos.symbol) != currentPrices.end()) {
            double currentPrice = currentPrices.at(pos.symbol);
            pos.currentValue = pos.quantity * currentPrice;
            pos.profitLoss = pos.currentValue - (pos.quantity * pos.averagePrice);
        }
    }
}

double Portfolio::getTotalValue(const std::map<std::string, double>& currentPrices) const {
    double total = cashBalance;
    
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        if (currentPrices.find(pos.symbol) != currentPrices.end()) {
            total += pos.quantity * currentPrices.at(pos.symbol);
        }
    }
    
    return total;
}

double Portfolio::getTotalProfitLoss() const {
    double total = 0.0;
    
    for (const auto& pair : positions) {
        total += pair.second.profitLoss;
    }
    
    return total;
}

Position* Portfolio::getPosition(const std::string& symbol) {
    if (positions.find(symbol) != positions.end()) {
        return &positions[symbol];
    }
    return nullptr;
}

void Portfolio::displayPositions() const {
    if (positions.empty()) {
        std::cout << "No positions in portfolio." << std::endl;
        return;
    }
    
    std::cout << "\n" << std::string(90, '=') << std::endl;
    std::cout << "PORTFOLIO POSITIONS" << std::endl;
    std::cout << std::string(90, '=') << std::endl;
    
    std::cout << std::left << std::setw(10) << "Symbol"
              << std::right << std::setw(10) << "Quantity"
              << std::setw(15) << "Avg Price"
              << std::setw(15) << "Current Val"
              << std::setw(15) << "P&L"
              << std::setw(12) << "P&L %" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        double plPercent = (pos.averagePrice > 0) ? 
            (pos.profitLoss / (pos.quantity * pos.averagePrice)) * 100.0 : 0.0;
        
        std::cout << std::left << std::setw(10) << pos.symbol
                  << std::right << std::setw(10) << pos.quantity
                  << std::setw(15) << std::fixed << std::setprecision(2) << pos.averagePrice
                  << std::setw(15) << pos.currentValue
                  << std::setw(15) << pos.profitLoss
                  << std::setw(11) << plPercent << "%" << std::endl;
    }
    
    std::cout << std::string(90, '=') << std::endl;
    std::cout << "Cash Balance: $" << std::fixed << std::setprecision(2) << cashBalance << std::endl;
    std::cout << "Total P&L: $" << getTotalProfitLoss() << std::endl;
}

void Portfolio::displayTransactionHistory(int limit) const {
    if (transactionHistory.empty()) {
        std::cout << "No transaction history." << std::endl;
        return;
    }
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "TRANSACTION HISTORY (Last " << limit << ")" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << std::left << std::setw(20) << "Time"
              << std::setw(8) << "Type"
              << std::setw(10) << "Symbol"
              << std::right << std::setw(10) << "Quantity"
              << std::setw(12) << "Price"
              << std::setw(15) << "Total" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    int start = std::max(0, static_cast<int>(transactionHistory.size()) - limit);
    
    for (int i = transactionHistory.size() - 1; i >= start; i--) {
        const Transaction& txn = transactionHistory[i];
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&txn.timestamp));
        
        std::cout << std::left << std::setw(20) << timeStr
                  << std::setw(8) << txn.type
                  << std::setw(10) << txn.symbol
                  << std::right << std::setw(10) << txn.quantity
                  << std::setw(12) << std::fixed << std::setprecision(2) << txn.price
                  << std::setw(15) << (txn.quantity * txn.price) << std::endl;
    }
    
    std::cout << std::string(80, '=') << std::endl;
}

std::string Portfolio::serialize() const {
    std::ostringstream oss;
    oss << userId << "|" << cashBalance << "|";
    
    // Serialize positions
    oss << positions.size() << "|";
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        oss << pos.symbol << "," << pos.quantity << "," << pos.averagePrice << ";";
    }
    
    // Serialize transactions
    oss << "|" << transactionHistory.size() << "|";
    for (const auto& txn : transactionHistory) {
        oss << txn.serialize() << ";";
    }
    
    return oss.str();
}

Portfolio Portfolio::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string userId, balanceStr, posCountStr, posData, txnCountStr, txnData;
    
    std::getline(iss, userId, '|');
    std::getline(iss, balanceStr, '|');
    std::getline(iss, posCountStr, '|');
    std::getline(iss, posData, '|');
    std::getline(iss, txnCountStr, '|');
    std::getline(iss, txnData);
    
    Portfolio portfolio(userId, std::stod(balanceStr));
    
    // Deserialize positions
    int posCount = std::stoi(posCountStr);
    std::istringstream posStream(posData);
    std::string posItem;
    
    for (int i = 0; i < posCount && std::getline(posStream, posItem, ';'); i++) {
        if (posItem.empty()) continue;
        
        std::istringstream posItemStream(posItem);
        std::string symbol, qtyStr, priceStr;
        
        std::getline(posItemStream, symbol, ',');
        std::getline(posItemStream, qtyStr, ',');
        std::getline(posItemStream, priceStr);
        
        portfolio.positions[symbol] = Position(symbol, std::stoi(qtyStr), std::stod(priceStr));
    }
    
    // Deserialize transactions
    int txnCount = std::stoi(txnCountStr);
    std::istringstream txnStream(txnData);
    std::string txnItem;
    
    for (int i = 0; i < txnCount && std::getline(txnStream, txnItem, ';'); i++) {
        if (txnItem.empty()) continue;
        portfolio.transactionHistory.push_back(Transaction::deserialize(txnItem));
    }
    
    return portfolio;
}
