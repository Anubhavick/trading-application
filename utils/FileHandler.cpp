#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

FileHandler::FileHandler(const std::string& dataDir)
    : dataDirectory(dataDir) {
    ensureDataDirectory();
}

std::string FileHandler::getUsersFilePath() const {
    return dataDirectory + "/users.txt";
}

std::string FileHandler::getStocksFilePath() const {
    return dataDirectory + "/stocks.txt";
}

std::string FileHandler::getPortfolioFilePath(const std::string& username) const {
    return dataDirectory + "/portfolio_" + username + ".txt";
}

void FileHandler::ensureDataDirectory() {
    struct stat info;
    if (stat(dataDirectory.c_str(), &info) != 0) {
        // Directory doesn't exist, create it
#ifdef _WIN32
        mkdir(dataDirectory.c_str());
#else
        mkdir(dataDirectory.c_str(), 0755);
#endif
    }
}

bool FileHandler::fileExists(const std::string& filepath) const {
    std::ifstream file(filepath);
    return file.good();
}

bool FileHandler::saveUser(const User& user) {
    std::ofstream file(getUsersFilePath(), std::ios::app);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open users file for writing." << std::endl;
        return false;
    }
    
    file << user.serialize() << std::endl;
    file.close();
    
    return true;
}

std::vector<std::unique_ptr<User>> FileHandler::loadAllUsers() {
    std::vector<std::unique_ptr<User>> users;
    
    if (!fileExists(getUsersFilePath())) {
        // Create default admin account
        std::unique_ptr<User> admin(new Admin("admin", "admin123"));
        saveUser(*admin);
        users.push_back(std::move(admin));
        return users;
    }
    
    std::ifstream file(getUsersFilePath());
    if (!file.is_open()) {
        std::cerr << "Error: Could not open users file for reading." << std::endl;
        return users;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string type;
        std::getline(iss, type, '|');
        
        if (type == "ADMIN") {
            users.push_back(std::unique_ptr<User>(new Admin(Admin::deserialize(line))));
        } else if (type == "TRADER") {
            users.push_back(std::unique_ptr<User>(new Trader(Trader::deserialize(line))));
        }
    }
    
    file.close();
    return users;
}

bool FileHandler::userExists(const std::string& username) {
    auto users = loadAllUsers();
    
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            return true;
        }
    }
    
    return false;
}

bool FileHandler::saveStocks(const TradingEngine& engine) {
    std::ofstream file(getStocksFilePath());
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open stocks file for writing." << std::endl;
        return false;
    }
    
    file << engine.serializeStocks();
    file.close();
    
    return true;
}

bool FileHandler::loadStocks(TradingEngine& engine) {
    if (!fileExists(getStocksFilePath())) {
        // Use default stocks from TradingEngine constructor
        return true;
    }
    
    std::ifstream file(getStocksFilePath());
    if (!file.is_open()) {
        std::cerr << "Error: Could not open stocks file for reading." << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    engine.deserializeStocks(buffer.str());
    
    return true;
}

bool FileHandler::savePortfolio(const Trader& trader) {
    std::ofstream file(getPortfolioFilePath(trader.getUsername()));
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open portfolio file for writing." << std::endl;
        return false;
    }
    
    file << trader.getPortfolio().serialize() << std::endl;
    file.close();
    
    return true;
}

bool FileHandler::loadPortfolio(Trader& trader) {
    std::string filepath = getPortfolioFilePath(trader.getUsername());
    
    if (!fileExists(filepath)) {
        // New trader, no portfolio file yet
        return true;
    }
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open portfolio file for reading." << std::endl;
        return false;
    }
    
    std::string line;
    if (std::getline(file, line)) {
        Portfolio portfolio = Portfolio::deserialize(line);
        trader.getPortfolio() = portfolio;
    }
    
    file.close();
    return true;
}
