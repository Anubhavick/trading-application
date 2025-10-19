#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <memory>
#include "../models/User.h"
#include "../services/TradingEngine.h"

// FileHandler manages data persistence
class FileHandler {
private:
    std::string dataDirectory;
    
    // File paths
    std::string getUsersFilePath() const;
    std::string getStocksFilePath() const;
    std::string getPortfolioFilePath(const std::string& username) const;

public:
    // Constructor
    FileHandler(const std::string& dataDir = "data");
    
    // User management
    bool saveUser(const User& user);
    std::vector<std::unique_ptr<User>> loadAllUsers();
    bool userExists(const std::string& username);
    
    // Stock management
    bool saveStocks(const TradingEngine& engine);
    bool loadStocks(TradingEngine& engine);
    
    // Portfolio management
    bool savePortfolio(const Trader& trader);
    bool loadPortfolio(Trader& trader);
    
    // Utility methods
    bool fileExists(const std::string& filepath) const;
    void ensureDataDirectory();
};

#endif
