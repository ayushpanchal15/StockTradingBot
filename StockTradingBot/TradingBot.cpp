#include "TradingBot.h"
#include <iostream>
#include <limits>
#include <deque>


TradingBot::TradingBot(double initialFunds)
    : bank(BankingSystem::getInstance(initialFunds)), selectedStock(""), profit(0.0) {
    std::cout << "TradingBot initialized with $" << initialFunds << " funds.\n";
}


TradingBot::~TradingBot() {
    // No need to delete 'bank' because it's managed by the singleton instance
}

void TradingBot::update(const std::string& stock, double price) {
    if (lastPrices.find(stock) != lastPrices.end()) {
        double change = price - lastPrices[stock];
        priceChanges[stock].push_back(change);

        // Maintain a fixed period (e.g., 14 updates)
        if (priceChanges[stock].size() > 14) {
            priceChanges[stock].erase(priceChanges[stock].begin());
        }
    }
    lastPrices[stock] = price; // Update last price
}

void TradingBot::addPriceToHistory(const std::string& stock, double price) {
    // Keep track of the last 20 prices for each stock
    if (priceHistory[stock].size() >= 20) {
        priceHistory[stock].pop_front(); // Remove the oldest price
    }
    priceHistory[stock].push_back(price);
}

void TradingBot::setSelectedStock(const std::string& stock) {
    selectedStock = stock;
    std::cout << "Selected stock for trading: " << selectedStock << "\n";
}

double TradingBot::calculateMomentum(const std::string& stock) {
    const int period = 5; // Short time frame for momentum calculation
    if (priceChanges[stock].size() < period) {
        return 0.0; // Not enough data
    }

    // Calculate the momentum by finding the difference between the current price and the price 'period' steps ago
    double momentum = priceChanges[stock].back() - priceChanges[stock][priceChanges[stock].size() - period];
    return momentum;
}

void TradingBot::executeTrade(const StockMarket& market) {
    if (selectedStock.empty() || market.getStocks().find(selectedStock) == market.getStocks().end()) {
        std::cout << "No valid stock selected for trading.\n";
        return;
    }

    double stockPrice = market.getStocks().at(selectedStock);
    double momentum = calculateMomentum(selectedStock);

    // Aggressive momentum trading thresholds
    const double buyThreshold = 0.5; // Positive momentum threshold for buying
    const double sellThreshold = -0.5; // Negative momentum threshold for selling

    // Buy logic (aggressive)
    if (portfolio[selectedStock] == 0 && momentum > buyThreshold) {
        double amountToInvest = 0.2 * bank->getBalance(); // Invest 20% of balance
        int unitsToBuy = static_cast<int>(amountToInvest / stockPrice);
        if (unitsToBuy > 0) {
            portfolio[selectedStock] += unitsToBuy;
            bank->withdraw(unitsToBuy * stockPrice);
            buyPrice = stockPrice;  // Store the buy price for later profit calculation
            std::cout << "Bought " << unitsToBuy << " units of " << selectedStock << " at $" << stockPrice << "\n";
        }
    }

    // Sell logic (aggressive)
    else if (portfolio[selectedStock] > 0 && momentum < sellThreshold) {
        int unitsToSell = portfolio[selectedStock];
        double totalSaleValue = unitsToSell * stockPrice;

        portfolio[selectedStock] -= unitsToSell;
        bank->deposit(totalSaleValue);

        // Calculate profit (difference between sale price and buy price)
        double tradeProfit = (stockPrice - buyPrice) * unitsToSell;
        profit += tradeProfit;

        std::cout << "Sold " << unitsToSell << " units of " << selectedStock << " at $" << stockPrice << "\n";
        std::cout << "Profit from this trade: $" << tradeProfit << "\n";
    }
}


void TradingBot::displayPortfolio() const {
    std::cout << "Portfolio:\n";
    for (const auto& [stock, quantity] : portfolio) {
        std::cout << stock << ": " << quantity << "\n";
    }
    std::cout << "Balance: " << bank->getBalance() << "\n";
}

double TradingBot::getCurrentBalance() const {
    return bank->getBalance();
}

double TradingBot::getTotalProfit() const {
    return profit;
}
