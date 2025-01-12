#include "BankingSystem.h"

BankingSystem* BankingSystem::instance = nullptr;

BankingSystem::BankingSystem(double initialFunds) : funds(initialFunds) {}

BankingSystem* BankingSystem::getInstance(double initialFunds) {
    if (!instance) {
        instance = new BankingSystem(initialFunds);
    }
    return instance;
}

void BankingSystem::deposit(double amount) {
    funds += amount;
}

bool BankingSystem::withdraw(double amount) {
    if (funds >= amount) {
        funds -= amount;
        return true;
    }
    return false;
}

double BankingSystem::getBalance() const {
    return funds;
}
