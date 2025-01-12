#include "StockMarket.h"
#include <cstdlib>
#include <ctime>

void StockMarket::addStock(const std::string& name, double price) {
    stocks[name] = price;
    previousPrices[name] = price;  // Initialize previous price
}

void StockMarket::updateStockPrice(const std::string& name, double newPrice) {
    stocks[name] = newPrice;
    for (auto observer : observers) {
        observer->update(name, newPrice);
    }
}

void StockMarket::simulatePriceFluctuations() {
    for (auto& [name, price] : stocks) {
        // Store the current price before updating (this becomes the previous price)
        previousPrices[name] = price;

        // Simulate price fluctuation (random change)
        price += (std::rand() % 200 - 100) / 100.0;  // Random price change between -1 and 1
        updateStockPrice(name, price);
    }
}

void StockMarket::attach(IObserver* observer) {
    observers.push_back(observer);
}

void StockMarket::detach(IObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

const std::unordered_map<std::string, double>& StockMarket::getStocks() const {
    return stocks;
}

double StockMarket::getPreviousPrice(const std::string& stockName) const {
    // If the stock exists in previousPrices, return the stored price, else return 0.0
    if (previousPrices.find(stockName) != previousPrices.end()) {
        return previousPrices.at(stockName);
    }
    return 0.0;  // If stock not found, return a default value of 0.0
}
