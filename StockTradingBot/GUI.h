#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include "StockMarket.h"
#include "TradingBot.h"

class GUI : public QWidget {
    Q_OBJECT

public:
    GUI(StockMarket* market, TradingBot* bot, QWidget* parent = nullptr);

private:
    StockMarket* market;
    TradingBot* bot;
    double balance;
    double profit;
    bool tradingActive;  // Declare tradingActive here

    QTableWidget* stockTable;
    QLabel* balanceLabel;
    QLabel* profitLabel;

    // Declare necessary methods
    void startAutomatedTrading();
    void simulateMarket();
    void updateMarketView();
    void updateBalanceAndProfit();
    void endTrading();  // Declare endTrading method
};

#endif // GUI_H
