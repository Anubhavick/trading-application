# OOP Concepts Demonstration Guide

This document explains how each Object-Oriented Programming concept is implemented in the Trading Application.

## 📦 1. Encapsulation

**Definition**: Bundling data and methods that operate on that data within a single unit (class), while hiding internal details.

### Examples in the Project:

#### Stock Class (`models/Stock.h`)
```cpp
class Stock {
private:
    std::string symbol;           // Hidden from outside
    std::string name;
    double currentPrice;
    std::vector<double> priceHistory;
    
public:
    // Controlled access through public methods
    std::string getSymbol() const;
    double getCurrentPrice() const;
    void setCurrentPrice(double price);
};
```

**Benefits**:
- Data integrity: Price can only be changed through `setCurrentPrice()`, which can validate input
- Implementation hiding: Price history vector is private; only exposed through methods
- Flexibility: Internal representation can change without affecting users of the class

#### Portfolio Class (`models/Portfolio.h`)
```cpp
class Portfolio {
private:
    std::string userId;
    double cashBalance;                          // Protected data
    std::map<std::string, Position> positions;   // Internal implementation hidden
    
public:
    double getCashBalance() const;               // Read-only access
    bool buyStock(...);                          // Controlled modification
    bool sellStock(...);                         // With validation
};
```

**Key Points**:
- Private data members protect state
- Public methods provide controlled interface
- Validation logic hidden inside methods

---

## 🌳 2. Inheritance

**Definition**: Creating new classes (derived) from existing classes (base), inheriting properties and behaviors.

### Class Hierarchies:

#### User Hierarchy (`models/User.h`)
```cpp
// Base class
class User {
protected:
    std::string username;
    std::string password;
    std::string role;
    
public:
    virtual void displayMenu() const = 0;  // Pure virtual
};

// Derived class 1
class Admin : public User {
public:
    Admin(const std::string& username, const std::string& password);
    void displayMenu() const override;  // Custom implementation
};

// Derived class 2
class Trader : public User {
private:
    Portfolio portfolio;  // Trader-specific data
    
public:
    void displayMenu() const override;  // Different implementation
    Portfolio& getPortfolio();
};
```

**Demonstrates**:
- "Is-a" relationship: Admin IS-A User, Trader IS-A User
- Code reuse: Both Admin and Trader inherit common User functionality
- Specialization: Each derived class adds unique features

#### Order Hierarchy (`models/Order.h`)
```cpp
// Abstract base class
class Order {
protected:
    std::string orderId;
    std::string symbol;
    int quantity;
    double price;
    
public:
    virtual std::string getOrderType() const = 0;  // Pure virtual
    virtual void display() const;                  // Virtual (can override)
};

// Concrete derived classes
class BuyOrder : public Order {
public:
    std::string getOrderType() const override { return "BUY"; }
    void display() const override;
};

class SellOrder : public Order {
public:
    std::string getOrderType() const override { return "SELL"; }
    void display() const override;
};
```

**Benefits**:
- Eliminates code duplication
- Creates logical hierarchies
- Enables polymorphic behavior

---

## 🎭 3. Polymorphism

**Definition**: The ability of objects of different classes to respond to the same method call in their own way.

### Runtime Polymorphism (Dynamic Binding):

#### Order Processing (`services/TradingEngine.cpp`)
```cpp
bool TradingEngine::executeOrder(Order* order, Portfolio& portfolio) {
    // Runtime determination of order type
    if (BuyOrder* buyOrder = dynamic_cast<BuyOrder*>(order)) {
        return executeBuyOrder(buyOrder, portfolio);
    } else if (SellOrder* sellOrder = dynamic_cast<SellOrder*>(order)) {
        return executeSellOrder(sellOrder, portfolio);
    }
    return false;
}
```

**How it works**:
- Single interface (`Order*`) handles multiple types
- Actual type determined at runtime
- Each order type executes differently

#### Strategy Pattern (`services/StrategyEngine.h`)
```cpp
// Base strategy class
class TradingStrategy {
public:
    virtual std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) = 0;
};

// Multiple implementations
class BuyBelowPriceStrategy : public TradingStrategy {
    std::vector<std::unique_ptr<Order>> generateSignals(...) override {
        // Buy logic
    }
};

class MovingAverageCrossoverStrategy : public TradingStrategy {
    std::vector<std::unique_ptr<Order>> generateSignals(...) override {
        // MA crossover logic
    }
};

class MeanReversionStrategy : public TradingStrategy {
    std::vector<std::unique_ptr<Order>> generateSignals(...) override {
        // Mean reversion logic
    }
};
```

**Usage**:
```cpp
// Polymorphic call - actual behavior depends on object type
TradingStrategy* strategy = strategies[index].get();
auto signals = strategy->generateSignals(stocks, portfolio);
```

#### User Menu Display (`main.cpp`)
```cpp
User* currentUser = authenticateUser();  // Could be Admin or Trader

// Polymorphic call - displays appropriate menu
currentUser->displayMenu();  // Admin menu OR Trader menu

// Different behavior based on runtime type
if (currentUser->getRole() == "ADMIN") {
    handleAdminSession(admin, ...);
} else {
    handleTraderSession(trader, ...);
}
```

**Benefits**:
- Write generic code that works with multiple types
- Easy to extend with new types
- Cleaner, more maintainable code

---

## 🎨 4. Abstraction

**Definition**: Hiding complex implementation details and showing only essential features.

### Abstract Base Classes:

#### User Abstract Class (`models/User.h`)
```cpp
class User {
protected:
    std::string username;
    std::string password;
    
public:
    // Pure virtual function - must be implemented by derived classes
    virtual void displayMenu() const = 0;
    
    // Virtual destructor for proper cleanup
    virtual ~User() = default;
    
    // Common interface
    std::string getUsername() const;
    bool verifyPassword(const std::string& pass) const;
};
```

**Key Points**:
- Cannot instantiate User directly (abstract class)
- Forces derived classes to implement `displayMenu()`
- Provides common interface for all user types

#### Order Abstract Class (`models/Order.h`)
```cpp
class Order {
public:
    // Pure virtual - defines contract
    virtual std::string getOrderType() const = 0;
    
    // Virtual - can be overridden
    virtual void display() const;
    
    // Common functionality
    double getTotalValue() const { return quantity * price; }
};
```

**Benefits**:
- Defines clear contracts (interfaces)
- Hides implementation complexity
- Promotes consistent design

#### Strategy Abstract Class (`services/StrategyEngine.h`)
```cpp
class TradingStrategy {
protected:
    std::string strategyName;
    std::string description;
    
public:
    // Pure virtual - each strategy must implement
    virtual std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) = 0;
    
    // Common interface
    std::string getStrategyName() const;
    virtual void displayInfo() const;
};
```

**Usage**:
- Strategy users don't need to know HOW signals are generated
- They only need to know THAT signals are generated
- Implementation details are hidden

---

## 🧩 5. Composition

**Definition**: Building complex objects by combining simpler objects ("has-a" relationship).

### Examples:

#### Trader Has-A Portfolio (`models/User.h`)
```cpp
class Trader : public User {
private:
    Portfolio portfolio;  // Composition - Trader HAS-A Portfolio
    
public:
    Portfolio& getPortfolio() { return portfolio; }
};
```

**Relationship**:
- Trader cannot exist without Portfolio
- Portfolio is part of Trader's identity
- Lifetime: Portfolio created when Trader is created

#### Portfolio Has-A Positions (`models/Portfolio.h`)
```cpp
class Portfolio {
private:
    std::map<std::string, Position> positions;  // Composition
    std::vector<Transaction> transactionHistory;  // Composition
    
public:
    bool buyStock(...) {
        // Operates on composed objects
        positions[symbol] = Position(...);
        transactionHistory.push_back(Transaction(...));
    }
};
```

**Benefits**:
- Models real-world relationships
- Better than inheritance when relationship is "has-a" not "is-a"
- More flexible than inheritance

#### TradingEngine Has-A Stocks (`services/TradingEngine.h`)
```cpp
class TradingEngine {
private:
    std::map<std::string, Stock> stocks;  // Composition
    std::vector<std::unique_ptr<Order>> orderBook;  // Composition
    
public:
    void addStock(const Stock& stock);
    Stock* getStock(const std::string& symbol);
};
```

#### StrategyEngine Has-A Strategies (`services/StrategyEngine.h`)
```cpp
class StrategyEngine {
private:
    std::vector<std::unique_ptr<TradingStrategy>> strategies;  // Composition
    
public:
    void addStrategy(std::unique_ptr<TradingStrategy> strategy);
    std::vector<std::unique_ptr<Order>> runStrategy(int index, ...);
};
```

---

## 🏗️ Additional Design Patterns

### Strategy Pattern

**Location**: `services/StrategyEngine.h`

**Implementation**:
```cpp
// Context
class StrategyEngine {
    std::vector<std::unique_ptr<TradingStrategy>> strategies;
public:
    std::vector<Order*> runStrategy(int index, ...);
};

// Strategy interface
class TradingStrategy { virtual generateSignals() = 0; };

// Concrete strategies
class BuyBelowPriceStrategy : public TradingStrategy { ... };
class MovingAverageCrossoverStrategy : public TradingStrategy { ... };
```

**Benefits**:
- Easy to add new strategies
- Strategies interchangeable at runtime
- Each strategy encapsulated

### Factory Pattern

**Location**: Throughout the codebase

**Example**:
```cpp
// Creating different order types
Order* order = nullptr;
if (type == "BUY") {
    order = new BuyOrder(symbol, quantity, price);
} else {
    order = new SellOrder(symbol, quantity, price);
}
```

---

## 📊 Class Relationships Diagram

```
User (Abstract)
├── Admin
└── Trader
    └── HAS-A Portfolio
              ├── HAS-A Position(s)
              └── HAS-A Transaction(s)

Order (Abstract)
├── BuyOrder
└── SellOrder

TradingStrategy (Abstract)
├── BuyBelowPriceStrategy
├── MovingAverageCrossoverStrategy
└── MeanReversionStrategy

TradingEngine
└── HAS-A Stock(s)

StrategyEngine
└── HAS-A TradingStrategy(s)
```

---

## 🎯 SOLID Principles Applied

### Single Responsibility Principle (SRP)
- Each class has one clear purpose
- `Stock`: Manages stock data
- `Portfolio`: Manages positions
- `TradingEngine`: Executes trades
- `FileHandler`: Handles persistence

### Open/Closed Principle (OCP)
- Open for extension, closed for modification
- Add new strategies without changing StrategyEngine
- Add new order types without changing Order base class

### Liskov Substitution Principle (LSP)
- Derived classes can substitute base classes
- `BuyOrder` and `SellOrder` work wherever `Order*` is expected
- `Admin` and `Trader` work wherever `User*` is expected

### Interface Segregation Principle (ISP)
- Clients depend only on methods they use
- `Order` interface focused on order operations
- `User` interface focused on authentication and menu

### Dependency Inversion Principle (DIP)
- Depend on abstractions, not concrete classes
- `TradingEngine` works with `Order*` (abstract)
- `StrategyEngine` works with `TradingStrategy*` (abstract)

---

## 🔍 Key Takeaways

1. **Encapsulation** protects data and provides controlled access
2. **Inheritance** enables code reuse and creates logical hierarchies
3. **Polymorphism** allows objects to behave differently through the same interface
4. **Abstraction** hides complexity and defines clear contracts
5. **Composition** builds complex objects from simpler ones

All these concepts work together to create a maintainable, extensible, and well-designed system!
