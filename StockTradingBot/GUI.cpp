#include "GUI.h"
#include <QHeaderView>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <iostream>

GUI::GUI(StockMarket* market, TradingBot* bot, QWidget* parent)
    : QWidget(parent), market(market), bot(bot), balance(0.0), profit(0.0), tradingActive(false) {

    // Ensure market and bot are valid before proceeding
    if (!market || !bot) {
        std::cerr << "Error: market or bot is null!" << std::endl;
        return;
    }

    // Set a dark background for the window
    setStyleSheet("background-color: #121212; color: #FFFFFF;");

    QVBoxLayout* layout = new QVBoxLayout(this);

    stockTable = new QTableWidget(this);
    stockTable->setColumnCount(3);
    stockTable->setHorizontalHeaderLabels({"Stock", "Price", "Change"});

    // Change header font and color
    QFont headerFont;
    headerFont.setBold(true);
    headerFont.setPointSize(12);
    stockTable->horizontalHeader()->setFont(headerFont);
    stockTable->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #333333; color: white; padding: 5px; }");

    // Set stock table background and text color
    stockTable->setStyleSheet("QTableWidget { background-color: #1F1F1F; color: white; font-size: 12pt; }");

    stockTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(stockTable);

    // Balance and Profit Labels with updated colors
    balanceLabel = new QLabel("Balance: $0.00", this);
    profitLabel = new QLabel("Profit: $0.00", this);
    balanceLabel->setStyleSheet("color: #00FF00; font-size: 14pt;");
    profitLabel->setStyleSheet("color: #FF4500; font-size: 14pt;");

    layout->addWidget(balanceLabel);
    layout->addWidget(profitLabel);

    // Start button with a new color and cursor change
    QPushButton* startButton = new QPushButton("Start Trading", this);
    startButton->setStyleSheet("QPushButton { background-color: #00BFFF; color: white; border: none; padding: 10px; border-radius: 5px; font-size: 14pt; }"
                               "QPushButton:hover { background-color: #1E90FF; }");
    layout->addWidget(startButton);

    // End button with a new color and cursor change
    QPushButton* endButton = new QPushButton("End Trading", this);
    endButton->setStyleSheet("QPushButton { background-color: #FF6347; color: white; border: none; padding: 10px; border-radius: 5px; font-size: 14pt; }"
                             "QPushButton:hover { background-color: #FF4500; }");
    layout->addWidget(endButton);

    // Set cursor to hand when hovering over the buttons
    startButton->setCursor(Qt::PointingHandCursor);
    endButton->setCursor(Qt::PointingHandCursor);

    connect(startButton, &QPushButton::clicked, this, &GUI::startAutomatedTrading);
    connect(endButton, &QPushButton::clicked, this, &GUI::endTrading);
}

void GUI::startAutomatedTrading() {
    // Ensure market and bot are valid before starting trading
    if (!market || !bot) {
        std::cerr << "Error: market or bot is null during trading start!" << std::endl;
        return;
    }

    // Check if trading is already active
    if (tradingActive) {
        std::cout << "Trading is already active!" << std::endl;
        return;
    }

    // Start trading by setting the flag to true
    tradingActive = true;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GUI::simulateMarket);
    timer->start(1000); // Update every second
}

void GUI::endTrading() {
    // Stop trading
    tradingActive = false;

    // Display the final balance and profit
    updateBalanceAndProfit();

    // Optionally, show a message about trading end
    std::cout << "Trading ended." << std::endl;
    std::cout << "Final Balance: $" << balance << std::endl;
    std::cout << "Total Profit: $" << profit << std::endl;
}

void GUI::simulateMarket() {
    // Ensure market and bot are valid before simulating
    if (!market || !bot) {
        std::cerr << "Error: market or bot is null during market simulation!" << std::endl;
        return;
    }

    // Only simulate if trading is active
    if (!tradingActive) {
        return;
    }

    // Simulate market price fluctuations and execute trade
    market->simulatePriceFluctuations(); // Ensure this method is implemented
    bot->executeTrade(*market);          // Ensure this method works correctly

    updateMarketView();
    updateBalanceAndProfit();
}

void GUI::updateMarketView() {
    // Ensure market is valid before updating the view
    if (!market) {
        std::cerr << "Error: market is null when updating view!" << std::endl;
        return;
    }

    const auto& stocks = market->getStocks();
    stockTable->setRowCount(static_cast<int>(stocks.size()));

    int row = 0;
    for (const auto& [stockName, price] : stocks) {
        stockTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(stockName)));
        stockTable->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 2)));

        // Get the price change by comparing with the previous price
        double change = price - market->getPreviousPrice(stockName); // Ensure this method exists and works
        QString changeText = QString::number(change, 'f', 2);

        QTableWidgetItem* changeItem = new QTableWidgetItem(changeText);

        // Change color based on price movement
        if (change > 0) {
            changeItem->setBackgroundColor(Qt::green);  // Positive change (price went up)
        } else if (change < 0) {
            changeItem->setBackgroundColor(Qt::red);    // Negative change (price went down)
        } else {
            changeItem->setBackgroundColor(Qt::white);  // No change
        }

        stockTable->setItem(row, 2, changeItem);
        ++row;
    }
}

void GUI::updateBalanceAndProfit() {
    // Ensure bot is valid before accessing balance and profit
    if (!bot) {
        std::cerr << "Error: bot is null when updating balance and profit!" << std::endl;
        return;
    }

    balance = bot->getCurrentBalance();
    profit = bot->getTotalProfit();

    balanceLabel->setText("Balance: $" + QString::number(balance, 'f', 2));
    profitLabel->setText("Profit: $" + QString::number(profit, 'f', 2));
}
