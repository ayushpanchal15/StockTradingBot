#ifndef TRADINGBOT_H
#define TRADINGBOT_H

#include "StockMarket.h"
#include "BankingSystem.h"
#include <unordered_map>
#include <string>
#include <deque>


class TradingBot : public IObserver {
private:
    BankingSystem* bank;
    std::string selectedStock; // The stock selected by the user
    std::unordered_map<std::string, int> portfolio;
    std::unordered_map<std::string, std::vector<double>> priceChanges;
    std::unordered_map<std::string, double> lastPrices;
    std::unordered_map<std::string, std::deque<double>> priceHistory;
    double buyPrice;
    double profit;

public:
    TradingBot(double initialFunds);
    virtual ~TradingBot();

    void addPriceToHistory(const std::string& stock, double price);
    double calculateMomentum(const std::string& stock);
    void setSelectedStock(const std::string& stock); // Set the stock to trade
    void update(const std::string& stock, double price) override;
    void executeTrade(const StockMarket& market);
    void displayPortfolio() const;
    double getCurrentBalance() const;
    double getTotalProfit() const;
};

#endif // TRADINGBOT_H
