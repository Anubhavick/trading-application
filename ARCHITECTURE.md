# Trading Application - Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         MAIN APPLICATION                        │
│                          (main.cpp)                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│  │ Welcome      │  │ Login/       │  │ Session      │         │
│  │ Screen       │→ │ Register     │→ │ Management   │         │
│  └──────────────┘  └──────────────┘  └──────────────┘         │
└─────────────────────────────────────────────────────────────────┘
                            ↓
    ┌───────────────────────┴───────────────────────┐
    ↓                                               ↓
┌─────────────────────────┐           ┌─────────────────────────┐
│    ADMIN SESSION        │           │    TRADER SESSION       │
│  ┌──────────────────┐   │           │  ┌──────────────────┐   │
│  │ Stock Management │   │           │  │ View Market      │   │
│  │ - Add Stock      │   │           │  │ - Stock Prices   │   │
│  │ - Remove Stock   │   │           │  │ - Details        │   │
│  └──────────────────┘   │           │  └──────────────────┘   │
│  ┌──────────────────┐   │           │  ┌──────────────────┐   │
│  │ Price Simulation │   │           │  │ Trading          │   │
│  │ - Normal Market  │   │           │  │ - Buy Orders     │   │
│  │ - Bull Market    │   │           │  │ - Sell Orders    │   │
│  │ - Bear Market    │   │           │  └──────────────────┘   │
│  │ - Volatile       │   │           │  ┌──────────────────┐   │
│  └──────────────────┘   │           │  │ Portfolio        │   │
│  ┌──────────────────┐   │           │  │ - View Positions │   │
│  │ User Management  │   │           │  │ - P&L Tracking   │   │
│  │ - View Users     │   │           │  │ - Transactions   │   │
│  │ - Statistics     │   │           │  └──────────────────┘   │
│  └──────────────────┘   │           │  ┌──────────────────┐   │
└─────────────────────────┘           │  │ Strategies       │   │
                                      │  │ - Buy Below      │   │
                                      │  │ - MA Crossover   │   │
                                      │  │ - Mean Reversion │   │
                                      │  └──────────────────┘   │
                                      └─────────────────────────┘
                                      
┌─────────────────────────────────────────────────────────────────┐
│                         SERVICES LAYER                          │
├─────────────────────────────────────────────────────────────────┤
│  ┌──────────────────────────────┐  ┌────────────────────────┐  │
│  │     TradingEngine            │  │   StrategyEngine       │  │
│  │  ┌────────────────────────┐  │  │  ┌──────────────────┐  │  │
│  │  │ Stock Management       │  │  │  │ Strategy Mgmt    │  │  │
│  │  │ - getAllStocks()       │  │  │  │ - addStrategy()  │  │  │
│  │  │ - addStock()           │  │  │  │ - runStrategy()  │  │  │
│  │  │ - removeStock()        │  │  │  └──────────────────┘  │  │
│  │  └────────────────────────┘  │  │  ┌──────────────────┐  │  │
│  │  ┌────────────────────────┐  │  │  │ Strategies       │  │  │
│  │  │ Order Execution        │  │  │  │ - BuyBelowPrice  │  │  │
│  │  │ - executeOrder()       │  │  │  │ - MA Crossover   │  │  │
│  │  │ - executeBuyOrder()    │  │  │  │ - MeanReversion  │  │  │
│  │  │ - executeSellOrder()   │  │  │  └──────────────────┘  │  │
│  │  └────────────────────────┘  │  └────────────────────────┘  │
│  └──────────────────────────────┘                              │
└─────────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────────┐
│                         MODELS LAYER                            │
├─────────────────────────────────────────────────────────────────┤
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐       │
│  │  User    │  │  Order   │  │  Stock   │  │Portfolio │       │
│  │ (Base)   │  │ (Base)   │  │          │  │          │       │
│  │          │  │          │  │ -symbol  │  │ -userId  │       │
│  │-username │  │-orderId  │  │ -name    │  │ -balance │       │
│  │-password │  │-symbol   │  │ -price   │  │ -positions│      │
│  │-role     │  │-quantity │  │ -history │  │ -txns    │       │
│  └────┬─────┘  └────┬─────┘  └──────────┘  └──────────┘       │
│       │             │                                           │
│   ┌───┴───┐     ┌───┴───┐                                      │
│   ↓       ↓     ↓       ↓                                      │
│ Admin  Trader  Buy    Sell                                     │
│               Order  Order                                      │
│                                                                 │
│  ┌──────────────────────────────────────────────────┐          │
│  │            TradingStrategy (Base)                │          │
│  │  - generateSignals()                             │          │
│  └────┬─────────────┬─────────────┬─────────────────┘          │
│       ↓             ↓             ↓                            │
│  BuyBelow     MACrossover   MeanReversion                      │
│  Strategy      Strategy        Strategy                        │
└─────────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────────┐
│                         UTILS LAYER                             │
├─────────────────────────────────────────────────────────────────┤
│  ┌────────────────────────┐  ┌──────────────────────────┐      │
│  │    FileHandler         │  │    PriceSimulator        │      │
│  │  ┌──────────────────┐  │  │  ┌────────────────────┐  │      │
│  │  │ User Persistence │  │  │  │ Price Updates      │  │      │
│  │  │ - saveUser()     │  │  │  │ - simulate()       │  │      │
│  │  │ - loadAllUsers() │  │  │  │ - bullMarket()     │  │      │
│  │  └──────────────────┘  │  │  │ - bearMarket()     │  │      │
│  │  ┌──────────────────┐  │  │  │ - volatileMarket() │  │      │
│  │  │ Stock Persistence│  │  │  └────────────────────┘  │      │
│  │  │ - saveStocks()   │  │  └──────────────────────────┘      │
│  │  │ - loadStocks()   │  │                                    │
│  │  └──────────────────┘  │                                    │
│  │  ┌──────────────────┐  │                                    │
│  │  │Portfolio Persist │  │                                    │
│  │  │ - savePortfolio()│  │                                    │
│  │  │ - loadPortfolio()│  │                                    │
│  │  └──────────────────┘  │                                    │
│  └────────────────────────┘                                    │
└─────────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────────┐
│                      DATA PERSISTENCE                           │
├─────────────────────────────────────────────────────────────────┤
│  data/                                                          │
│  ├── users.txt              (All user accounts)                 │
│  ├── stocks.txt             (Stock market data)                 │
│  └── portfolio_*.txt        (Individual portfolios)             │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      OOP CONCEPTS MAP                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ENCAPSULATION: All classes (private data, public methods)     │
│  ├─ Stock: Protected price data                                │
│  ├─ Portfolio: Protected balance & positions                   │
│  └─ User: Protected credentials                                │
│                                                                 │
│  INHERITANCE: Multiple hierarchies                             │
│  ├─ User → Admin, Trader                                       │
│  ├─ Order → BuyOrder, SellOrder                                │
│  └─ TradingStrategy → 3 concrete strategies                    │
│                                                                 │
│  POLYMORPHISM: Runtime behavior                                │
│  ├─ Order execution (Buy vs Sell)                              │
│  ├─ User menus (Admin vs Trader)                               │
│  └─ Strategy execution (different algorithms)                  │
│                                                                 │
│  ABSTRACTION: Abstract base classes                            │
│  ├─ User (pure virtual displayMenu)                            │
│  ├─ Order (pure virtual getOrderType)                          │
│  └─ TradingStrategy (pure virtual generateSignals)             │
│                                                                 │
│  COMPOSITION: Has-a relationships                              │
│  ├─ Trader HAS-A Portfolio                                     │
│  ├─ Portfolio HAS-A Positions                                  │
│  ├─ TradingEngine HAS-A Stocks                                 │
│  └─ StrategyEngine HAS-A Strategies                            │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘

FLOW: User Login → Role Check → Session Handler → 
      Services → Models → Data Persistence
```

## Key Relationships

### Inheritance Hierarchies
```
User                Order              TradingStrategy
  ├─ Admin            ├─ BuyOrder         ├─ BuyBelowPrice
  └─ Trader           └─ SellOrder        ├─ MACrossover
                                          └─ MeanReversion
```

### Composition Relationships
```
Trader ──has-a──> Portfolio ──has-a──> Position(s)
                              │
                              └──has-a──> Transaction(s)

TradingEngine ──has-a──> Stock(s)
StrategyEngine ──has-a──> TradingStrategy(s)
```

### Dependencies
```
Main → FileHandler → Models
Main → TradingEngine → Models
Main → StrategyEngine → Models
TradingEngine ← PriceSimulator
```

## Data Flow

1. **User Authentication**: Main → FileHandler → User models
2. **Trading**: Trader → TradingEngine → Order → Portfolio → Stock
3. **Strategy**: Trader → StrategyEngine → Strategy → Order
4. **Price Updates**: Admin → PriceSimulator → Stock
5. **Persistence**: Any component → FileHandler → File system
