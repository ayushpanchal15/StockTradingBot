cmake_minimum_required(VERSION 3.10)
project(StockTradingBot)

set(CMAKE_CXX_STANDARD 17)

# Specify the path to Qt5 installation
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")
set(CMAKE_AUTOMOC ON)

# Find the Qt5 Widgets module
find_package(Qt5Widgets REQUIRED)

add_executable(StockTradingBot main.cpp StockMarket.cpp BankingSystem.cpp TradingBot.cpp GUI.cpp)

# Link to Qt5 Widgets
target_link_libraries(StockTradingBot Qt5::Widgets)
