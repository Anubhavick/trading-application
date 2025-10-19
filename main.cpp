#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>

#include "models/Stock.h"
#include "models/Order.h"
#include "models/Portfolio.h"
#include "models/User.h"
#include "services/TradingEngine.h"
#include "services/StrategyEngine.h"
#include "utils/FileHandler.h"
#include "utils/PriceSimulator.h"

// Forward declarations
void clearScreen();
void pauseScreen();
void displayWelcome();
User* authenticateUser(FileHandler& fileHandler);
User* registerUser(FileHandler& fileHandler);
void handleAdminSession(Admin* admin, TradingEngine& engine, PriceSimulator& simulator, 
                        FileHandler& fileHandler, std::vector<std::unique_ptr<User>>& users);
void handleTraderSession(Trader* trader, TradingEngine& engine, StrategyEngine& strategyEngine,
                         FileHandler& fileHandler);

// Utility functions
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void displayWelcome() {
    clearScreen();
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "     WELCOME TO THE TRADING APPLICATION" << std::endl;
    std::cout << "     Advanced C++ OOP Project" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << "  - Real-time stock trading simulation" << std::endl;
    std::cout << "  - Portfolio management" << std::endl;
    std::cout << "  - Automated trading strategies" << std::endl;
    std::cout << "  - Transaction history tracking" << std::endl;
    std::cout << "  - Admin controls for market management" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

// Authentication
User* authenticateUser(FileHandler& fileHandler) {
    std::string username, password;
    
    std::cout << "\n=== LOGIN ===" << std::endl;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    
    auto users = fileHandler.loadAllUsers();
    
    for (auto& user : users) {
        if (user->getUsername() == username && user->verifyPassword(password)) {
            std::cout << "\nLogin successful! Welcome, " << username << std::endl;
            
            // Return a pointer to the appropriate user type
            if (user->getRole() == "ADMIN") {
                Admin* admin = dynamic_cast<Admin*>(user.get());
                return new Admin(*admin);
            } else if (user->getRole() == "TRADER") {
                Trader* trader = dynamic_cast<Trader*>(user.get());
                Trader* newTrader = new Trader(*trader);
                fileHandler.loadPortfolio(*newTrader);
                return newTrader;
            }
        }
    }
    
    std::cout << "\nInvalid username or password." << std::endl;
    return nullptr;
}

User* registerUser(FileHandler& fileHandler) {
    std::string username, password, confirmPassword;
    int roleChoice;
    
    std::cout << "\n=== REGISTRATION ===" << std::endl;
    std::cout << "Username: ";
    std::cin >> username;
    
    if (fileHandler.userExists(username)) {
        std::cout << "Username already exists!" << std::endl;
        return nullptr;
    }
    
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "Confirm Password: ";
    std::cin >> confirmPassword;
    
    if (password != confirmPassword) {
        std::cout << "Passwords do not match!" << std::endl;
        return nullptr;
    }
    
    std::cout << "\nSelect role:" << std::endl;
    std::cout << "1. Trader (default)" << std::endl;
    std::cout << "2. Admin" << std::endl;
    std::cout << "Choice: ";
    std::cin >> roleChoice;
    
    User* newUser = nullptr;
    
    if (roleChoice == 2) {
        newUser = new Admin(username, password);
        std::cout << "\nAdmin account created successfully!" << std::endl;
    } else {
        newUser = new Trader(username, password, 100000.0);
        std::cout << "\nTrader account created with $100,000 initial balance!" << std::endl;
    }
    
    fileHandler.saveUser(*newUser);
    return newUser;
}

// Admin session handler
void handleAdminSession(Admin* admin, TradingEngine& engine, PriceSimulator& simulator,
                        FileHandler& fileHandler, std::vector<std::unique_ptr<User>>& users) {
    bool running = true;
    
    while (running) {
        clearScreen();
        admin->displayMenu();
        
        int choice;
        std::cout << "\nEnter choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1: { // View all stocks
                clearScreen();
                engine.displayMarket();
                pauseScreen();
                break;
            }
            
            case 2: { // Add new stock
                clearScreen();
                std::string symbol, name;
                double price;
                
                std::cout << "=== ADD NEW STOCK ===" << std::endl;
                std::cout << "Symbol: ";
                std::cin >> symbol;
                std::cin.ignore();
                std::cout << "Company Name: ";
                std::getline(std::cin, name);
                std::cout << "Initial Price: $";
                std::cin >> price;
                
                Stock newStock(symbol, name, price);
                engine.addStock(newStock);
                fileHandler.saveStocks(engine);
                
                pauseScreen();
                break;
            }
            
            case 3: { // Remove stock
                clearScreen();
                std::string symbol;
                
                engine.displayMarket();
                std::cout << "\nEnter symbol to remove: ";
                std::cin >> symbol;
                
                engine.removeStock(symbol);
                fileHandler.saveStocks(engine);
                
                pauseScreen();
                break;
            }
            
            case 4: { // Simulate price changes
                clearScreen();
                std::cout << "=== PRICE SIMULATION ===" << std::endl;
                std::cout << "1. Normal Market Update" << std::endl;
                std::cout << "2. Bull Market (Upward Trend)" << std::endl;
                std::cout << "3. Bear Market (Downward Trend)" << std::endl;
                std::cout << "4. Volatile Market" << std::endl;
                
                int simChoice;
                std::cout << "\nChoice: ";
                std::cin >> simChoice;
                
                auto& stocks = const_cast<std::map<std::string, Stock>&>(engine.getAllStocks());
                
                switch (simChoice) {
                    case 1:
                        simulator.simulateMarket(stocks);
                        break;
                    case 2:
                        simulator.simulateBullMarket(stocks);
                        break;
                    case 3:
                        simulator.simulateBearMarket(stocks);
                        break;
                    case 4:
                        simulator.simulateVolatileMarket(stocks);
                        break;
                }
                
                fileHandler.saveStocks(engine);
                pauseScreen();
                break;
            }
            
            case 5: { // View all users
                clearScreen();
                users = fileHandler.loadAllUsers();
                
                std::cout << "\n=== REGISTERED USERS ===" << std::endl;
                std::cout << std::string(50, '=') << std::endl;
                std::cout << std::left << std::setw(20) << "Username" 
                          << std::setw(15) << "Role" << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                for (const auto& user : users) {
                    std::cout << std::left << std::setw(20) << user->getUsername()
                              << std::setw(15) << user->getRole() << std::endl;
                }
                
                std::cout << std::string(50, '=') << std::endl;
                pauseScreen();
                break;
            }
            
            case 6: { // System statistics
                clearScreen();
                users = fileHandler.loadAllUsers();
                
                int adminCount = 0, traderCount = 0;
                for (const auto& user : users) {
                    if (user->getRole() == "ADMIN") adminCount++;
                    else if (user->getRole() == "TRADER") traderCount++;
                }
                
                std::cout << "\n=== SYSTEM STATISTICS ===" << std::endl;
                std::cout << std::string(50, '=') << std::endl;
                std::cout << "Total Users: " << users.size() << std::endl;
                std::cout << "  - Admins: " << adminCount << std::endl;
                std::cout << "  - Traders: " << traderCount << std::endl;
                std::cout << "Total Stocks: " << engine.getAllStocks().size() << std::endl;
                std::cout << std::string(50, '=') << std::endl;
                
                pauseScreen();
                break;
            }
            
            case 7: { // Logout
                running = false;
                std::cout << "\nLogging out..." << std::endl;
                break;
            }
            
            default:
                std::cout << "Invalid choice!" << std::endl;
                pauseScreen();
        }
    }
}

// Trader session handler
void handleTraderSession(Trader* trader, TradingEngine& engine, StrategyEngine& strategyEngine,
                         FileHandler& fileHandler) {
    bool running = true;
    
    while (running) {
        clearScreen();
        trader->displayMenu();
        
        // Update portfolio values
        trader->getPortfolio().updatePositionValues(engine.getCurrentPrices());
        
        int choice;
        std::cout << "\nEnter choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1: { // View stock market
                clearScreen();
                engine.displayMarket();
                
                std::cout << "\nView stock details? (Enter symbol or 'n' to skip): ";
                std::string symbol;
                std::cin >> symbol;
                
                if (symbol != "n" && symbol != "N") {
                    engine.displayStockDetails(symbol);
                }
                
                pauseScreen();
                break;
            }
            
            case 2: { // Place buy order
                clearScreen();
                engine.displayMarket();
                
                std::string symbol;
                int quantity;
                
                std::cout << "\n=== PLACE BUY ORDER ===" << std::endl;
                std::cout << "Symbol: ";
                std::cin >> symbol;
                std::cout << "Quantity: ";
                std::cin >> quantity;
                
                Stock* stock = engine.getStock(symbol);
                if (stock) {
                    BuyOrder order(symbol, quantity, stock->getCurrentPrice());
                    engine.executeOrder(&order, trader->getPortfolio());
                    fileHandler.savePortfolio(*trader);
                } else {
                    std::cout << "Stock not found!" << std::endl;
                }
                
                pauseScreen();
                break;
            }
            
            case 3: { // Place sell order
                clearScreen();
                trader->getPortfolio().displayPositions();
                
                std::string symbol;
                int quantity;
                
                std::cout << "\n=== PLACE SELL ORDER ===" << std::endl;
                std::cout << "Symbol: ";
                std::cin >> symbol;
                std::cout << "Quantity: ";
                std::cin >> quantity;
                
                Stock* stock = engine.getStock(symbol);
                if (stock) {
                    SellOrder order(symbol, quantity, stock->getCurrentPrice());
                    engine.executeOrder(&order, trader->getPortfolio());
                    fileHandler.savePortfolio(*trader);
                } else {
                    std::cout << "Stock not found!" << std::endl;
                }
                
                pauseScreen();
                break;
            }
            
            case 4: { // View portfolio
                clearScreen();
                trader->getPortfolio().displayPositions();
                
                double totalValue = trader->getPortfolio().getTotalValue(engine.getCurrentPrices());
                std::cout << "\nTotal Portfolio Value: $" << std::fixed << std::setprecision(2) 
                          << totalValue << std::endl;
                
                pauseScreen();
                break;
            }
            
            case 5: { // View transaction history
                clearScreen();
                trader->getPortfolio().displayTransactionHistory(20);
                pauseScreen();
                break;
            }
            
            case 6: { // Run trading strategy
                clearScreen();
                strategyEngine.displayStrategies();
                
                int strategyChoice;
                std::cout << "\nSelect strategy (0 to cancel): ";
                std::cin >> strategyChoice;
                
                if (strategyChoice > 0 && 
                    static_cast<size_t>(strategyChoice) <= strategyEngine.getStrategies().size()) {
                    auto signals = strategyEngine.runStrategy(
                        strategyChoice - 1, 
                        engine.getAllStocks(), 
                        trader->getPortfolio()
                    );
                    
                    if (!signals.empty()) {
                        std::cout << "\nExecute these orders? (y/n): ";
                        char confirm;
                        std::cin >> confirm;
                        
                        if (confirm == 'y' || confirm == 'Y') {
                            for (auto& order : signals) {
                                engine.executeOrder(order.get(), trader->getPortfolio());
                            }
                            fileHandler.savePortfolio(*trader);
                        }
                    }
                }
                
                pauseScreen();
                break;
            }
            
            case 7: { // Account summary
                clearScreen();
                trader->displayTraderInfo();
                
                double totalValue = trader->getPortfolio().getTotalValue(engine.getCurrentPrices());
                double profitLoss = trader->getPortfolio().getTotalProfitLoss();
                
                std::cout << "\n=== PORTFOLIO SUMMARY ===" << std::endl;
                std::cout << "Total Value: $" << std::fixed << std::setprecision(2) 
                          << totalValue << std::endl;
                std::cout << "Total P&L: $" << profitLoss << std::endl;
                std::cout << "Number of Positions: " 
                          << trader->getPortfolio().getPositions().size() << std::endl;
                std::cout << "Total Transactions: " 
                          << trader->getPortfolio().getTransactionHistory().size() << std::endl;
                
                pauseScreen();
                break;
            }
            
            case 8: { // Logout
                running = false;
                fileHandler.savePortfolio(*trader);
                std::cout << "\nLogging out and saving portfolio..." << std::endl;
                break;
            }
            
            default:
                std::cout << "Invalid choice!" << std::endl;
                pauseScreen();
        }
    }
}

// Main function
int main() {
    // Initialize components
    FileHandler fileHandler("data");
    TradingEngine engine;
    StrategyEngine strategyEngine;
    PriceSimulator simulator(0.02, 0.0001);
    
    // Load stocks from file
    fileHandler.loadStocks(engine);
    
    // Main application loop
    bool running = true;
    
    while (running) {
        displayWelcome();
        
        std::cout << "\n1. Login" << std::endl;
        std::cout << "2. Register" << std::endl;
        std::cout << "3. Exit" << std::endl;
        
        int choice;
        std::cout << "\nEnter choice: ";
        std::cin >> choice;
        
        User* currentUser = nullptr;
        std::vector<std::unique_ptr<User>> users;
        
        switch (choice) {
            case 1: { // Login
                currentUser = authenticateUser(fileHandler);
                
                if (currentUser) {
                    if (currentUser->getRole() == "ADMIN") {
                        Admin* admin = dynamic_cast<Admin*>(currentUser);
                        handleAdminSession(admin, engine, simulator, fileHandler, users);
                    } else if (currentUser->getRole() == "TRADER") {
                        Trader* trader = dynamic_cast<Trader*>(currentUser);
                        handleTraderSession(trader, engine, strategyEngine, fileHandler);
                    }
                    
                    delete currentUser;
                } else {
                    pauseScreen();
                }
                
                break;
            }
            
            case 2: { // Register
                currentUser = registerUser(fileHandler);
                
                if (currentUser) {
                    pauseScreen();
                    delete currentUser;
                } else {
                    pauseScreen();
                }
                
                break;
            }
            
            case 3: { // Exit
                running = false;
                std::cout << "\nThank you for using the Trading Application!" << std::endl;
                std::cout << "Goodbye!" << std::endl;
                break;
            }
            
            default:
                std::cout << "Invalid choice!" << std::endl;
                pauseScreen();
        }
    }
    
    return 0;
}
