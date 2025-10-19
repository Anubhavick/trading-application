#include "User.h"
#include <iostream>
#include <sstream>

// Base User implementation
User::User(const std::string& username, const std::string& password, const std::string& role)
    : username(username), password(password), role(role) {}

std::string User::getUsername() const {
    return username;
}

std::string User::getRole() const {
    return role;
}

bool User::verifyPassword(const std::string& pass) const {
    return password == pass;
}

std::string User::serialize() const {
    return username + "|" + password + "|" + role;
}

// Admin implementation
Admin::Admin(const std::string& username, const std::string& password)
    : User(username, password, "ADMIN") {}

void Admin::displayMenu() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ADMIN MENU - Welcome, " << username << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1. View All Stocks" << std::endl;
    std::cout << "2. Add New Stock" << std::endl;
    std::cout << "3. Remove Stock" << std::endl;
    std::cout << "4. Simulate Price Changes" << std::endl;
    std::cout << "5. View All Users" << std::endl;
    std::cout << "6. System Statistics" << std::endl;
    std::cout << "7. Logout" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void Admin::displayAdminInfo() const {
    std::cout << "\n=== Admin Account ===" << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Role: " << role << std::endl;
}

std::string Admin::serialize() const {
    return "ADMIN|" + User::serialize();
}

Admin Admin::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type, username, password, role;
    
    std::getline(iss, type, '|');
    std::getline(iss, username, '|');
    std::getline(iss, password, '|');
    std::getline(iss, role);
    
    return Admin(username, password);
}

// Trader implementation
Trader::Trader(const std::string& username, const std::string& password, double initialBalance)
    : User(username, password, "TRADER"), portfolio(username, initialBalance) {}

void Trader::displayMenu() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TRADER MENU - Welcome, " << username << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1. View Stock Market" << std::endl;
    std::cout << "2. Place Buy Order" << std::endl;
    std::cout << "3. Place Sell Order" << std::endl;
    std::cout << "4. View Portfolio" << std::endl;
    std::cout << "5. View Transaction History" << std::endl;
    std::cout << "6. Run Trading Strategy" << std::endl;
    std::cout << "7. Account Summary" << std::endl;
    std::cout << "8. Logout" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

Portfolio& Trader::getPortfolio() {
    return portfolio;
}

const Portfolio& Trader::getPortfolio() const {
    return portfolio;
}

void Trader::displayTraderInfo() const {
    std::cout << "\n=== Trader Account ===" << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Role: " << role << std::endl;
    std::cout << "Cash Balance: $" << portfolio.getCashBalance() << std::endl;
}

std::string Trader::serialize() const {
    return "TRADER|" + User::serialize() + "|" + portfolio.serialize();
}

Trader Trader::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type, username, password, role, portfolioData;
    
    std::getline(iss, type, '|');
    std::getline(iss, username, '|');
    std::getline(iss, password, '|');
    std::getline(iss, role, '|');
    
    // Get remaining data for portfolio
    std::getline(iss, portfolioData);
    
    Trader trader(username, password, 0.0);
    trader.portfolio = Portfolio::deserialize(portfolioData);
    
    return trader;
}
