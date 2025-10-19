#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ctime>

// Abstract base class demonstrating Abstraction and Polymorphism
class Order {
protected:
    std::string orderId;
    std::string symbol;
    int quantity;
    double price;
    time_t timestamp;
    std::string status; // "PENDING", "EXECUTED", "CANCELLED"

public:
    // Constructor
    Order(const std::string& symbol, int quantity, double price);
    
    // Virtual destructor for proper cleanup
    virtual ~Order() = default;
    
    // Pure virtual function - makes this an abstract class
    virtual std::string getOrderType() const = 0;
    
    // Virtual function for polymorphism
    virtual void display() const;
    
    // Getters
    std::string getOrderId() const;
    std::string getSymbol() const;
    int getQuantity() const;
    double getPrice() const;
    double getTotalValue() const;
    std::string getStatus() const;
    time_t getTimestamp() const;
    
    // Setters
    void setStatus(const std::string& newStatus);
    
    // Serialization
    virtual std::string serialize() const;
    
protected:
    std::string generateOrderId();
};

// Derived class demonstrating Inheritance
class BuyOrder : public Order {
public:
    BuyOrder(const std::string& symbol, int quantity, double price);
    
    // Override virtual function
    std::string getOrderType() const override;
    void display() const override;
    
    std::string serialize() const override;
    static BuyOrder deserialize(const std::string& data);
};

// Another derived class demonstrating Inheritance
class SellOrder : public Order {
public:
    SellOrder(const std::string& symbol, int quantity, double price);
    
    // Override virtual function
    std::string getOrderType() const override;
    void display() const override;
    
    std::string serialize() const override;
    static SellOrder deserialize(const std::string& data);
};

#endif
