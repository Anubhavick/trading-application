#ifndef USER_H
#define USER_H

#include <string>
#include "Portfolio.h"

// Base User class demonstrating Abstraction and Encapsulation
class User {
protected:
    std::string username;
    std::string password;
    std::string role; // "ADMIN" or "TRADER"

public:
    // Constructor
    User(const std::string& username, const std::string& password, const std::string& role);
    
    // Virtual destructor
    virtual ~User() = default;
    
    // Pure virtual function - makes User abstract
    virtual void displayMenu() const = 0;
    
    // Getters (Encapsulation)
    std::string getUsername() const;
    std::string getRole() const;
    bool verifyPassword(const std::string& pass) const;
    
    // Serialization
    virtual std::string serialize() const;
};

// Admin class demonstrating Inheritance
class Admin : public User {
public:
    // Constructor
    Admin(const std::string& username, const std::string& password);
    
    // Override abstract method
    void displayMenu() const override;
    
    // Admin-specific functionality
    void displayAdminInfo() const;
    
    // Serialization
    std::string serialize() const override;
    static Admin deserialize(const std::string& data);
};

// Trader class demonstrating Inheritance and Composition
class Trader : public User {
private:
    Portfolio portfolio; // Composition - Trader "has-a" Portfolio

public:
    // Constructor
    Trader(const std::string& username, const std::string& password, 
           double initialBalance = 100000.0);
    
    // Override abstract method
    void displayMenu() const override;
    
    // Trader-specific functionality
    Portfolio& getPortfolio();
    const Portfolio& getPortfolio() const;
    void displayTraderInfo() const;
    
    // Serialization
    std::string serialize() const override;
    static Trader deserialize(const std::string& data);
};

#endif
