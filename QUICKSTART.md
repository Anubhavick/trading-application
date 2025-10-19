# Trading Application - Quick Start Guide

## Build & Run

### Quick Start (Recommended)
```bash
make run
```

### Manual Build Steps
```bash
# Build the application
make

# Run the application
./trading_app
```

### Clean Build
```bash
make clean    # Remove object files and executable
make cleanall # Remove all including data files
```

## Default Login Credentials

**Admin Account:**
- Username: `admin`
- Password: `admin123`

## Creating Your First Trade

1. **Register a Trader Account**
   - Choose option 2 (Register)
   - Enter username and password
   - Select role 1 (Trader)
   - You'll start with $100,000

2. **View the Market**
   - Login with your credentials
   - Choose option 1 (View Stock Market)
   - Note stock symbols and prices

3. **Place Your First Buy Order**
   - Choose option 2 (Place Buy Order)
   - Enter stock symbol (e.g., AAPL)
   - Enter quantity (e.g., 10)
   - Order executes at current market price

4. **View Your Portfolio**
   - Choose option 4 (View Portfolio)
   - See your positions, P&L, and cash balance

5. **Try a Trading Strategy**
   - Choose option 6 (Run Trading Strategy)
   - Select a strategy (1, 2, or 3)
   - Review generated signals
   - Confirm execution (y/n)

## Available Stocks

- **AAPL** - Apple Inc.
- **GOOGL** - Alphabet Inc.
- **MSFT** - Microsoft Corp.
- **TSLA** - Tesla Inc.
- **AMZN** - Amazon.com Inc.

## Trading Strategies

### 1. Buy Below Price Strategy
- Buys stocks when price falls below $200
- Good for value investing approach

### 2. Moving Average Crossover
- Buys when 5-period MA crosses above 20-period MA
- Sells when 5-period MA crosses below 20-period MA
- Classic momentum strategy

### 3. Mean Reversion Strategy
- Buys when price is 5% below 20-period average
- Sells when price is 5% above 20-period average
- Profits from price corrections

## Admin Functions

### Simulating Market Conditions

1. **Normal Market Update**
   - Random price changes with realistic volatility
   - Use regularly to create trading opportunities

2. **Bull Market**
   - Creates upward price trends
   - Good for testing long positions

3. **Bear Market**
   - Creates downward price trends
   - Good for testing sell strategies

4. **Volatile Market**
   - High price fluctuations
   - Good for testing strategy robustness

### Adding Custom Stocks

1. Login as admin
2. Choose option 2 (Add New Stock)
3. Enter symbol (e.g., NFLX)
4. Enter company name (e.g., Netflix Inc.)
5. Enter initial price (e.g., 450.00)

## Tips for Best Experience

1. **Start Small**: Begin with small quantities (5-10 shares) to learn the system

2. **Update Prices Regularly**: As admin, simulate market changes to see strategies in action

3. **Monitor Your Portfolio**: Check P&L after market updates to track performance

4. **Try Different Strategies**: Each strategy works better in different market conditions

5. **Review Transaction History**: Learn from past trades (option 5 in trader menu)

## Common Workflows

### Testing a Trading Strategy
```
1. Login as admin
2. Simulate bull market (creates buying opportunities)
3. Logout and login as trader
4. Run "Buy Below Price" strategy
5. Execute generated signals
6. Login as admin again
7. Simulate normal market updates (let strategy play out)
8. Login as trader
9. View portfolio to see results
```

### Building a Diversified Portfolio
```
1. Login as trader
2. Buy 5-10 shares of each available stock
3. Use portfolio view to track total exposure
4. Rebalance by selling overperforming stocks
5. Use cash to buy underperforming stocks
```

## File Locations

- **User Data**: `data/users.txt`
- **Stock Data**: `data/stocks.txt`
- **Portfolio Data**: `data/portfolio_[username].txt`

## Troubleshooting

**"Insufficient funds" error**
- Check cash balance in account summary
- Sell some positions to free up capital
- Or reduce order quantity

**"You don't own any shares" error**
- You must own stock before selling
- Check portfolio to see current positions

**Strategy generates no signals**
- Market conditions may not match strategy criteria
- As admin, simulate different market conditions
- Try a different strategy

**Login fails**
- Check username and password are correct
- Default admin credentials: admin/admin123
- Register new account if needed

## OOP Concepts Demonstrated

This project showcases all major OOP principles:

1. **Encapsulation**: Private data members with public interfaces
2. **Inheritance**: User → Admin/Trader, Order → BuyOrder/SellOrder
3. **Polymorphism**: Virtual functions for different user/order/strategy behaviors
4. **Abstraction**: Abstract base classes (User, Order, TradingStrategy)
5. **Composition**: Trader has-a Portfolio, Portfolio has-a Positions

## Need Help?

- Check `README.md` for detailed documentation
- Review code comments for implementation details
- All classes follow clear OOP design patterns

---

**Happy Trading! 📈**
