#ifndef STOCKMARKET_H
#define STOCKMARKET_H

#include <string>
#include <unordered_map>
#include <vector>

class IObserver {
public:
    virtual void update(const std::string& stock, double price) = 0;
};

class StockMarket {
private:
    std::unordered_map<std::string, double> stocks;                // Current stock prices
    std::vector<IObserver*> observers;                              // List of observers
    std::unordered_map<std::string, double> previousPrices;         // Previous stock prices (for tracking price changes)

public:
    void addStock(const std::string& name, double price);
    void updateStockPrice(const std::string& name, double newPrice);
    void simulatePriceFluctuations();
    void attach(IObserver* observer);
    void detach(IObserver* observer);
    const std::unordered_map<std::string, double>& getStocks() const;
    
    // New method to retrieve the previous price of a stock
    double getPreviousPrice(const std::string& stockName) const;
};

#endif // STOCKMARKET_H
