# C++ Trading Application

A comprehensive console-based trading application demonstrating advanced Object-Oriented Programming (OOP) concepts in C++. This project showcases inheritance, polymorphism, encapsulation, abstraction, and composition through a feature-rich stock trading simulator.

## 🎯 Project Overview

This trading application simulates a real-world stock trading platform where users can:
- Trade stocks in a simulated market
- Manage portfolios and track performance
- Execute automated trading strategies
- View real-time market data and price history
- Access role-based features (Admin vs Trader)

## 🏗️ Architecture & OOP Concepts

### **1. Encapsulation**
All classes use private/protected data members with public getter/setter methods:
- `Stock`, `Portfolio`, `User` classes encapsulate their internal state
- Data integrity is maintained through controlled access

### **2. Inheritance**
Clear hierarchical relationships demonstrating "is-a" relationships:
- `User` (base) → `Admin` and `Trader` (derived)
- `Order` (abstract base) → `BuyOrder` and `SellOrder` (concrete)
- `TradingStrategy` (abstract base) → Multiple strategy implementations

### **3. Polymorphism**
Runtime polymorphism through virtual functions:
- `Order::getOrderType()` - Different behavior for Buy/Sell orders
- `User::displayMenu()` - Different menus for Admin/Trader
- `TradingStrategy::generateSignals()` - Different strategy implementations

### **4. Abstraction**
Abstract base classes define interfaces:
- `Order` - Pure virtual methods define order contract
- `User` - Abstract base with pure virtual `displayMenu()`
- `TradingStrategy` - Strategy pattern for trading algorithms

### **5. Composition**
"Has-a" relationships for complex object composition:
- `Trader` has a `Portfolio`
- `Portfolio` has multiple `Position` objects
- `TradingEngine` has multiple `Stock` objects

## 📁 Project Structure

```
trading-application/
├── models/                 # Data models
│   ├── Stock.h/cpp        # Stock information and price history
│   ├── Order.h/cpp        # Order hierarchy (Buy/Sell)
│   ├── Portfolio.h/cpp    # Portfolio management and positions
│   └── User.h/cpp         # User hierarchy (Admin/Trader)
├── services/              # Business logic
│   ├── TradingEngine.h/cpp    # Order execution and stock management
│   └── StrategyEngine.h/cpp   # Trading strategy framework
├── utils/                 # Utility classes
│   ├── FileHandler.h/cpp      # Data persistence
│   └── PriceSimulator.h/cpp   # Market simulation
├── data/                  # Data storage (auto-generated)
├── main.cpp              # Application entry point
├── Makefile              # Build configuration (Make)
├── CMakeLists.txt        # Build configuration (CMake)
└── README.md             # This file
```

## ✨ Features

### **For Traders**
1. **View Stock Market** - Real-time stock prices with change indicators
2. **Place Buy/Sell Orders** - Execute trades at market prices
3. **Portfolio Management** - Track positions, P&L, and cash balance
4. **Transaction History** - Complete audit trail of all trades
5. **Trading Strategies** - Automated trading based on technical indicators:
   - Buy Below Price Strategy
   - Moving Average Crossover
   - Mean Reversion Strategy
6. **Account Summary** - Comprehensive portfolio performance metrics

### **For Admins**
1. **Stock Management** - Add/remove stocks from the market
2. **Price Simulation** - Simulate various market conditions:
   - Normal market updates
   - Bull market (upward trend)
   - Bear market (downward trend)
   - Volatile market (high fluctuation)
3. **User Management** - View all registered users
4. **System Statistics** - Monitor platform usage

### **General Features**
- User authentication (login/register)
- Role-based access control
- File-based data persistence
- Clean console UI with formatted tables
- Input validation and error handling

## 🚀 Getting Started

### Prerequisites
- C++ compiler with C++11 support or later (GCC 4.8+, Clang 3.3+, MSVC 2013+)
- Make (for Makefile build) or CMake 3.10+ (for CMake build)

### Building the Application

#### Option 1: Using Make
```bash
# Build the application
make

# Run the application
./trading_app

# Or build and run in one command
make run

# Clean build artifacts
make clean

# Clean all including data
make cleanall
```

#### Option 2: Using CMake
```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the application
cmake --build .

# Run the application
./trading_app
```

#### Option 3: Manual Compilation
```bash
# Compile all source files
g++ -std=c++11 -o trading_app \
    main.cpp \
    models/Stock.cpp \
    models/Order.cpp \
    models/Portfolio.cpp \
    models/User.cpp \
    services/TradingEngine.cpp \
    services/StrategyEngine.cpp \
    utils/FileHandler.cpp \
    utils/PriceSimulator.cpp

# Run the application
./trading_app
```

## 📖 Usage Guide

### First Time Setup
1. Run the application
2. Register a new user (choose Trader or Admin role)
3. Login with your credentials

### Default Credentials
- **Username:** `admin`
- **Password:** `admin123`
- **Role:** Admin

### Trading Workflow (Trader)
1. Login as a trader
2. View the stock market (option 1)
3. Place buy orders for desired stocks (option 2)
4. Monitor your portfolio (option 4)
5. Run trading strategies for automated decisions (option 6)
6. Place sell orders to realize profits (option 3)
7. View transaction history (option 5)

### Market Management (Admin)
1. Login as admin
2. Add new stocks to the market (option 2)
3. Simulate price changes:
   - Normal updates for regular volatility
   - Bull market to create buying opportunities
   - Bear market to test portfolio resilience
4. Monitor system statistics (option 6)

## 🎓 Educational Value

This project demonstrates:

### **Design Patterns**
- **Strategy Pattern**: Pluggable trading strategies
- **Factory Pattern**: Order creation (Buy/Sell)
- **Template Method**: User session handling

### **SOLID Principles**
- **Single Responsibility**: Each class has one clear purpose
- **Open/Closed**: Easily extend with new strategies or order types
- **Liskov Substitution**: Derived classes work seamlessly with base class pointers
- **Interface Segregation**: Focused interfaces for different roles
- **Dependency Inversion**: Depend on abstractions, not concrete classes

### **Memory Management**
- Smart pointers (`std::unique_ptr`, `std::make_unique`)
- Proper destructor implementation
- RAII principles

### **STL Usage**
- `std::vector`, `std::map` for collections
- `std::string`, `std::stringstream` for text processing
- Algorithms and iterators

## 📊 Sample Output

### Market View
```
================================================================================
STOCK MARKET
================================================================================
Symbol  Name                    Price      Change    Change%
--------------------------------------------------------------------------------
AAPL    Apple Inc.             150.50       2.35      1.59%
GOOGL   Alphabet Inc.         2800.75     -15.20     -0.54%
MSFT    Microsoft Corp.        310.25       5.80      1.90%
TSLA    Tesla Inc.             245.80      -3.45     -1.39%
AMZN    Amazon.com Inc.        135.40       1.20      0.89%
================================================================================
```

### Portfolio View
```
==========================================================================================
PORTFOLIO POSITIONS
==========================================================================================
Symbol      Quantity       Avg Price    Current Val            P&L       P&L %
------------------------------------------------------------------------------------------
AAPL              50          148.15         7525.00         117.50       1.59 %
MSFT             100          305.00        31025.00         525.00       1.72 %
==========================================================================================
Cash Balance: $61450.00
Total P&L: $642.50
```

## 🔧 Customization

### Adding New Trading Strategies
1. Create a new class inheriting from `TradingStrategy`
2. Implement `generateSignals()` method
3. Add to `StrategyEngine` constructor

```cpp
class MyCustomStrategy : public TradingStrategy {
public:
    MyCustomStrategy() : TradingStrategy("My Strategy", "Description") {}
    
    std::vector<std::unique_ptr<Order>> generateSignals(
        const std::map<std::string, Stock>& stocks,
        const Portfolio& portfolio) override {
        // Your strategy logic here
    }
};
```

### Adding New Stock Attributes
1. Add private members to `Stock` class
2. Update constructor and getter/setter methods
3. Update serialization methods
4. Modify display methods as needed

## 🐛 Troubleshooting

### Compilation Errors
- Ensure C++11 or later support: `-std=c++11`
- Check all header files are included
- Verify directory structure matches includes

### Runtime Issues
- Check `data/` directory permissions
- Verify sufficient disk space for data files
- Check input validation for numeric entries

## 📝 License

This project is created for educational purposes as part of an OOP course. Feel free to use, modify, and distribute for learning purposes.

## 👨‍💻 Author

Created as an advanced C++ OOP demonstration project.

## 🙏 Acknowledgments

- Demonstrates industry-standard C++ practices
- Follows modern C++ guidelines
- Implements real-world software design patterns
- Suitable for academic presentations and portfolios

---

**Note**: This is a simulation for educational purposes. Not intended for actual financial trading.
