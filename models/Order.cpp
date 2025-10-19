#include "Order.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

// Base Order implementation
Order::Order(const std::string& symbol, int quantity, double price)
    : symbol(symbol), quantity(quantity), price(price), 
      timestamp(std::time(nullptr)), status("PENDING") {
    orderId = generateOrderId();
}

std::string Order::generateOrderId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(10000, 99999);
    
    std::ostringstream oss;
    oss << "ORD" << dis(gen);
    return oss.str();
}

void Order::display() const {
    std::cout << std::left << std::setw(10) << orderId
              << std::setw(8) << getOrderType()
              << std::setw(8) << symbol
              << std::right << std::setw(8) << quantity
              << std::setw(12) << std::fixed << std::setprecision(2) << price
              << std::setw(12) << getTotalValue()
              << "  " << std::left << std::setw(10) << status;
}

std::string Order::getOrderId() const {
    return orderId;
}

std::string Order::getSymbol() const {
    return symbol;
}

int Order::getQuantity() const {
    return quantity;
}

double Order::getPrice() const {
    return price;
}

double Order::getTotalValue() const {
    return quantity * price;
}

std::string Order::getStatus() const {
    return status;
}

time_t Order::getTimestamp() const {
    return timestamp;
}

void Order::setStatus(const std::string& newStatus) {
    status = newStatus;
}

std::string Order::serialize() const {
    std::ostringstream oss;
    oss << orderId << "|" << symbol << "|" << quantity << "|" 
        << price << "|" << timestamp << "|" << status;
    return oss.str();
}

// BuyOrder implementation
BuyOrder::BuyOrder(const std::string& symbol, int quantity, double price)
    : Order(symbol, quantity, price) {}

std::string BuyOrder::getOrderType() const {
    return "BUY";
}

void BuyOrder::display() const {
    Order::display();
}

std::string BuyOrder::serialize() const {
    return "BUY|" + Order::serialize();
}

BuyOrder BuyOrder::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type, orderId, symbol, qtyStr, priceStr, timeStr, status;
    
    std::getline(iss, type, '|');
    std::getline(iss, orderId, '|');
    std::getline(iss, symbol, '|');
    std::getline(iss, qtyStr, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, timeStr, '|');
    std::getline(iss, status);
    
    BuyOrder order(symbol, std::stoi(qtyStr), std::stod(priceStr));
    order.orderId = orderId;
    order.timestamp = std::stol(timeStr);
    order.status = status;
    
    return order;
}

// SellOrder implementation
SellOrder::SellOrder(const std::string& symbol, int quantity, double price)
    : Order(symbol, quantity, price) {}

std::string SellOrder::getOrderType() const {
    return "SELL";
}

void SellOrder::display() const {
    Order::display();
}

std::string SellOrder::serialize() const {
    return "SELL|" + Order::serialize();
}

SellOrder SellOrder::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type, orderId, symbol, qtyStr, priceStr, timeStr, status;
    
    std::getline(iss, type, '|');
    std::getline(iss, orderId, '|');
    std::getline(iss, symbol, '|');
    std::getline(iss, qtyStr, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, timeStr, '|');
    std::getline(iss, status);
    
    SellOrder order(symbol, std::stoi(qtyStr), std::stod(priceStr));
    order.orderId = orderId;
    order.timestamp = std::stol(timeStr);
    order.status = status;
    
    return order;
}
