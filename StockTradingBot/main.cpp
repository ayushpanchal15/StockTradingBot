#include "StockMarket.h"
#include "TradingBot.h"
#include "GUI.h"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create StockMarket and TradingBot
    StockMarket market;
    double initialFunds = 10000.0; // Example initial fund
    TradingBot bot(initialFunds);

    // Attach the TradingBot to the StockMarket
    market.attach(&bot);

    // Add some stocks to the market
    market.addStock("AAPL", 150.0);
    market.addStock("MSFT", 330.0);
    market.addStock("TSLA", 700.0);

    // User selects the stock to trade
    std::string stockToTrade;
    std::cout << "Available stocks:\n";
    for (const auto& [symbol, price] : market.getStocks()) {
        std::cout << symbol << ": $" << price << "\n";
    }
    std::cout << "Enter the stock symbol you want to trade: ";
    std::cin >> stockToTrade;

    bot.setSelectedStock(stockToTrade);

    // Create and show the GUI
    GUI gui(&market, &bot);
    gui.show();

    return app.exec();
}
