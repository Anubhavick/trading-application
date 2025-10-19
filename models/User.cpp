#include "User.h"
#include "../utils/Console.h"
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
    std::cout << Color::BOLD << Color::BRIGHT_MAGENTA;
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║  " << Symbol::ADMIN << "  ADMIN MENU - Welcome, " << username << std::string(20 - username.length(), ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════╗\n";
    std::cout << Color::RESET;
    
    Console::printMenuOption(1, "View All Stocks", Symbol::CHART_UP);
    Console::printMenuOption(2, "Add New Stock", "➕");
    Console::printMenuOption(3, "Remove Stock", "➖");
    Console::printMenuOption(4, "Simulate Price Changes", Symbol::FIRE);
    Console::printMenuOption(5, "View All Users", Symbol::USER);
    Console::printMenuOption(6, "System Statistics", "📊");
    Console::printMenuOption(7, "Logout", "🚪");
    
    Console::printDivider("═", 50);
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
    std::cout << Color::BOLD << Color::BRIGHT_CYAN;
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║  " << Symbol::TRADER << "  TRADER MENU - Welcome, " << username << std::string(19 - username.length(), ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    std::cout << Color::RESET;
    
    Console::printMenuOption(1, "View Stock Market", Symbol::CHART_UP);
    Console::printMenuOption(2, "Place Buy Order", "🛒");
    Console::printMenuOption(3, "Place Sell Order", "💸");
    Console::printMenuOption(4, "View Portfolio", Symbol::MONEY_BAG);
    Console::printMenuOption(5, "View Transaction History", "📜");
    Console::printMenuOption(6, "Run Trading Strategy", Symbol::TARGET);
    Console::printMenuOption(7, "Account Summary", "📊");
    Console::printMenuOption(8, "Logout", "🚪");
    
    Console::printDivider("═", 50);
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
