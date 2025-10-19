# Trading Application - Architecture Documentation

## System Architecture

```mermaid
graph TB
    Main[main.cpp<br/>Entry Point] --> FileHandler[FileHandler<br/>Utils]
    Main --> TradingEngine[TradingEngine<br/>Services]
    Main --> StrategyEngine[StrategyEngine<br/>Services]
    Main --> PriceSimulator[PriceSimulator<br/>Utils]
    
    FileHandler --> DataFiles[Data Files<br/>users.txt<br/>stocks.txt<br/>portfolio_*.txt]
    
    TradingEngine --> Stock[Stock<br/>Models]
    TradingEngine --> Order[Order<br/>Models]
    TradingEngine --> Portfolio[Portfolio<br/>Models]
    
    StrategyEngine --> Strategies[Trading Strategies<br/>BuyBelow<br/>MA Crossover<br/>Mean Reversion]
    
    PriceSimulator --> Stock
    
    style Main fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style FileHandler fill:#7ac943,stroke:#5a9633,color:#fff
    style TradingEngine fill:#f39c12,stroke:#d68910,color:#fff
    style StrategyEngine fill:#f39c12,stroke:#d68910,color:#fff
    style PriceSimulator fill:#7ac943,stroke:#5a9633,color:#fff
    style Stock fill:#e74c3c,stroke:#c0392b,color:#fff
    style Order fill:#e74c3c,stroke:#c0392b,color:#fff
    style Portfolio fill:#e74c3c,stroke:#c0392b,color:#fff
    style DataFiles fill:#95a5a6,stroke:#7f8c8d,color:#fff
    style Strategies fill:#9b59b6,stroke:#8e44ad,color:#fff
```

## Class Hierarchy

```mermaid
classDiagram
    class User {
        <<abstract>>
        -string username
        -string password
        -string role
        +getUsername() string
        +verifyPassword() bool
        +displayMenu()* void
    }
    
    class Admin {
        +displayMenu() void
        +displayAdminInfo() void
    }
    
    class Trader {
        -Portfolio portfolio
        +displayMenu() void
        +getPortfolio() Portfolio
    }
    
    class Order {
        <<abstract>>
        #string orderId
        #string symbol
        #int quantity
        #double price
        +getOrderType()* string
        +display() void
    }
    
    class BuyOrder {
        +getOrderType() string
        +display() void
    }
    
    class SellOrder {
        +getOrderType() string
        +display() void
    }
    
    class TradingStrategy {
        <<abstract>>
        #string strategyName
        +generateSignals()* vector~Order~
        +displayInfo() void
    }
    
    class BuyBelowPriceStrategy {
        -double priceThreshold
        +generateSignals() vector~Order~
    }
    
    class MovingAverageCrossoverStrategy {
        -int shortPeriod
        -int longPeriod
        +generateSignals() vector~Order~
    }
    
    class MeanReversionStrategy {
        -int period
        -double threshold
        +generateSignals() vector~Order~
    }
    
    class Portfolio {
        -string userId
        -double cashBalance
        -map positions
        +buyStock() bool
        +sellStock() bool
        +displayPositions() void
    }
    
    User <|-- Admin
    User <|-- Trader
    Order <|-- BuyOrder
    Order <|-- SellOrder
    TradingStrategy <|-- BuyBelowPriceStrategy
    TradingStrategy <|-- MovingAverageCrossoverStrategy
    TradingStrategy <|-- MeanReversionStrategy
    Trader *-- Portfolio
```

## Data Flow

### User Authentication Flow

```mermaid
sequenceDiagram
    participant User
    participant Main
    participant FileHandler
    participant UserFactory
    
    User->>Main: Enter credentials
    Main->>FileHandler: loadAllUsers()
    FileHandler->>FileHandler: Read users.txt
    FileHandler-->>Main: Return user list
    Main->>Main: Verify password
    Main->>UserFactory: Create Admin/Trader
    UserFactory-->>Main: Return user object
    Main-->>User: Login successful
```

### Trading Flow (Trader)

```mermaid
flowchart TD
    Trader[Trader Session] --> ViewMarket{View Market}
    Trader --> PlaceOrder{Place Order}
    Trader --> ViewPortfolio{View Portfolio}
    Trader --> RunStrategy{Run Strategy}
    
    ViewMarket --> DisplayMarket[TradingEngine.displayMarket]
    DisplayMarket --> StockDisplay[Stock.display]
    
    PlaceOrder --> ExecuteOrder[TradingEngine.executeOrder]
    ExecuteOrder --> CheckType{Order Type?}
    CheckType -->|Buy| BuyStock[Portfolio.buyStock]
    CheckType -->|Sell| SellStock[Portfolio.sellStock]
    
    ViewPortfolio --> DisplayPos[Portfolio.displayPositions]
    
    RunStrategy --> StrategyEngine[StrategyEngine.runStrategy]
    StrategyEngine --> GenSignals[Strategy.generateSignals]
    GenSignals --> ExecuteOrders[Execute generated orders]
    
    style Trader fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style ViewMarket fill:#7ac943,stroke:#5a9633,color:#fff
    style PlaceOrder fill:#f39c12,stroke:#d68910,color:#fff
    style ViewPortfolio fill:#9b59b6,stroke:#8e44ad,color:#fff
    style RunStrategy fill:#e74c3c,stroke:#c0392b,color:#fff
```

### Market Management Flow (Admin)

```mermaid
flowchart TD
    Admin[Admin Session] --> AddStock{Add Stock}
    Admin --> RemoveStock{Remove Stock}
    Admin --> SimulatePrices{Simulate Prices}
    Admin --> ViewUsers{View Users}
    
    AddStock --> CreateStock[TradingEngine.addStock]
    CreateStock --> SaveStocks1[FileHandler.saveStocks]
    
    RemoveStock --> DeleteStock[TradingEngine.removeStock]
    DeleteStock --> SaveStocks2[FileHandler.saveStocks]
    
    SimulatePrices --> SimType{Simulation Type}
    SimType -->|Normal| NormalSim[PriceSimulator.simulateMarket]
    SimType -->|Bull| BullSim[PriceSimulator.simulateBullMarket]
    SimType -->|Bear| BearSim[PriceSimulator.simulateBearMarket]
    SimType -->|Volatile| VolSim[PriceSimulator.simulateVolatileMarket]
    
    NormalSim --> UpdatePrices[Stock.setCurrentPrice]
    BullSim --> UpdatePrices
    BearSim --> UpdatePrices
    VolSim --> UpdatePrices
    UpdatePrices --> SaveStocks3[FileHandler.saveStocks]
    
    ViewUsers --> LoadUsers[FileHandler.loadAllUsers]
    
    style Admin fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style AddStock fill:#7ac943,stroke:#5a9633,color:#fff
    style RemoveStock fill:#e74c3c,stroke:#c0392b,color:#fff
    style SimulatePrices fill:#f39c12,stroke:#d68910,color:#fff
    style ViewUsers fill:#9b59b6,stroke:#8e44ad,color:#fff
```

## Layered Architecture

```mermaid
graph TB
    subgraph Presentation["Presentation Layer"]
        UI[Console UI<br/>main.cpp<br/>Menus | Input | Output]
    end
    
    subgraph Business["Business Logic Layer"]
        TE[TradingEngine<br/>executeOrder<br/>addStock<br/>removeStock]
        SE[StrategyEngine<br/>runStrategy<br/>addStrategy]
        PS[PriceSimulator<br/>simulate<br/>update prices]
    end
    
    subgraph Models["Domain Model Layer"]
        Stock[Stock<br/>price<br/>history<br/>symbol]
        Order[Order<br/>symbol<br/>quantity<br/>price]
        Portfolio[Portfolio<br/>positions<br/>balance<br/>transactions]
        User[User<br/>credentials<br/>role]
    end
    
    subgraph Persistence["Persistence Layer"]
        FH[FileHandler<br/>save<br/>load<br/>serialize]
        Files[(Data Files<br/>users.txt<br/>stocks.txt<br/>portfolio_*.txt)]
    end
    
    UI --> TE
    UI --> SE
    UI --> FH
    
    TE --> Stock
    TE --> Order
    TE --> Portfolio
    
    SE --> Stock
    SE --> Order
    SE --> Portfolio
    
    PS --> Stock
    
    FH --> User
    FH --> Stock
    FH --> Portfolio
    FH --> Files
    
    style Presentation fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style Business fill:#f39c12,stroke:#d68910,color:#fff
    style Models fill:#e74c3c,stroke:#c0392b,color:#fff
    style Persistence fill:#7ac943,stroke:#5a9633,color:#fff
```

## OOP Concepts Implementation

### Encapsulation

```mermaid
classDiagram
    class Stock {
        -string symbol
        -string name
        -double currentPrice
        -vector priceHistory
        +getSymbol() string
        +getCurrentPrice() double
        +setCurrentPrice(price) void
        +display() void
    }
    
    class Portfolio {
        -string userId
        -double cashBalance
        -map positions
        -vector transactions
        +getCashBalance() double
        +buyStock(symbol, qty, price) bool
        +sellStock(symbol, qty, price) bool
        +displayPositions() void
    }
    
    note for Stock "Private data protected\nPublic interface only"
    note for Portfolio "Controlled access\nData validation in methods"
```

### Inheritance & Polymorphism

```mermaid
graph TB
    subgraph "User Hierarchy"
        User[User Abstract<br/>+displayMenu abstract] --> Admin[Admin<br/>+displayMenu override]
        User --> Trader[Trader<br/>+displayMenu override<br/>+portfolio]
    end
    
    subgraph "Order Hierarchy"
        Order[Order Abstract<br/>+getOrderType abstract] --> BuyOrder[BuyOrder<br/>+getOrderType: BUY]
        Order --> SellOrder[SellOrder<br/>+getOrderType: SELL]
    end
    
    subgraph "Strategy Hierarchy"
        Strategy[TradingStrategy Abstract<br/>+generateSignals abstract] --> BuyBelow[BuyBelowPriceStrategy]
        Strategy --> MACross[MovingAverageCrossoverStrategy]
        Strategy --> MeanRev[MeanReversionStrategy]
    end
    
    style User fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style Order fill:#f39c12,stroke:#d68910,color:#fff
    style Strategy fill:#e74c3c,stroke:#c0392b,color:#fff
```

### Composition Relationships

```mermaid
graph LR
    Trader[Trader] -->|has-a| Portfolio[Portfolio]
    Portfolio -->|has-many| Position[Position]
    Portfolio -->|has-many| Transaction[Transaction]
    
    TradingEngine[TradingEngine] -->|has-many| Stock[Stock]
    StrategyEngine[StrategyEngine] -->|has-many| TradingStrategy[TradingStrategy]
    
    style Trader fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style Portfolio fill:#f39c12,stroke:#d68910,color:#fff
    style Position fill:#7ac943,stroke:#5a9633,color:#fff
    style Transaction fill:#7ac943,stroke:#5a9633,color:#fff
    style TradingEngine fill:#e74c3c,stroke:#c0392b,color:#fff
    style Stock fill:#9b59b6,stroke:#8e44ad,color:#fff
```

## Module Dependencies

```mermaid
graph TD
    Main[main.cpp] --> FileHandler
    Main --> TradingEngine
    Main --> StrategyEngine
    Main --> Models[All Models]
    
    TradingEngine --> Stock
    TradingEngine --> Order
    TradingEngine --> Portfolio
    
    StrategyEngine --> Stock
    StrategyEngine --> Order
    StrategyEngine --> Portfolio
    
    FileHandler --> User
    FileHandler --> Stock
    FileHandler --> Portfolio
    
    PriceSimulator --> Stock
    
    style Main fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style FileHandler fill:#7ac943,stroke:#5a9633,color:#fff
    style TradingEngine fill:#f39c12,stroke:#d68910,color:#fff
    style StrategyEngine fill:#f39c12,stroke:#d68910,color:#fff
    style PriceSimulator fill:#7ac943,stroke:#5a9633,color:#fff
```

## Sequence Diagram: Execute Buy Order

```mermaid
sequenceDiagram
    participant Trader
    participant TradingEngine
    participant Portfolio
    participant Stock
    
    Trader->>TradingEngine: buy(AAPL, 10)
    TradingEngine->>Stock: getStock(AAPL)
    Stock-->>TradingEngine: currentPrice
    TradingEngine->>Portfolio: buyStock(AAPL, 10, price)
    Portfolio->>Portfolio: validate funds
    Portfolio->>Portfolio: deduct cash
    Portfolio->>Portfolio: add position
    Portfolio-->>TradingEngine: success
    TradingEngine-->>Trader: order executed
```

## File Structure

```mermaid
graph TD
    Root[trading-application/] --> Models[models/]
    Root --> Services[services/]
    Root --> Utils[utils/]
    Root --> Main[main.cpp]
    Root --> Data[data/]
    Root --> Docs[Documentation]
    
    Models --> Stock[Stock.h/cpp]
    Models --> Order[Order.h/cpp]
    Models --> Portfolio[Portfolio.h/cpp]
    Models --> User[User.h/cpp]
    
    Services --> TradingEngine[TradingEngine.h/cpp]
    Services --> StrategyEngine[StrategyEngine.h/cpp]
    
    Utils --> FileHandler[FileHandler.h/cpp]
    Utils --> PriceSimulator[PriceSimulator.h/cpp]
    Utils --> Colors[Colors.h]
    
    Data --> UsersFile[users.txt]
    Data --> StocksFile[stocks.txt]
    Data --> PortfolioFiles[portfolio_*.txt]
    
    Docs --> README[README.md]
    Docs --> QuickStart[QUICKSTART.md]
    Docs --> OOPConcepts[OOP_CONCEPTS.md]
    Docs --> Architecture[ARCHITECTURE.md]
    
    style Root fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style Models fill:#e74c3c,stroke:#c0392b,color:#fff
    style Services fill:#f39c12,stroke:#d68910,color:#fff
    style Utils fill:#7ac943,stroke:#5a9633,color:#fff
    style Main fill:#9b59b6,stroke:#8e44ad,color:#fff
    style Data fill:#95a5a6,stroke:#7f8c8d,color:#fff
    style Docs fill:#3498db,stroke:#2980b9,color:#fff
```

## Complete Data Flow Summary

```mermaid
graph LR
    A[User Login] --> B[FileHandler]
    B --> C[Load Users]
    C --> D{Verify}
    D -->|Admin| E[Admin Session]
    D -->|Trader| F[Trader Session]
    
    E --> G[Stock Management]
    E --> H[Price Simulation]
    
    F --> I[View Market]
    F --> J[Trading]
    F --> K[Strategies]
    
    G --> L[TradingEngine]
    H --> M[PriceSimulator]
    I --> L
    J --> L
    K --> N[StrategyEngine]
    
    L --> O[Models]
    M --> O
    N --> O
    
    O --> P[FileHandler Save]
    P --> Q[(Data Files)]
    
    style A fill:#4a90e2,stroke:#2e5c8a,color:#fff
    style E fill:#e74c3c,stroke:#c0392b,color:#fff
    style F fill:#7ac943,stroke:#5a9633,color:#fff
    style L fill:#f39c12,stroke:#d68910,color:#fff
    style N fill:#9b59b6,stroke:#8e44ad,color:#fff
```
