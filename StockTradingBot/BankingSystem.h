#ifndef BANKINGSYSTEM_H
#define BANKINGSYSTEM_H

class BankingSystem {
private:
    static BankingSystem* instance;
    double funds;

    BankingSystem(double initialFunds);

public:
    static BankingSystem* getInstance(double initialFunds = 0);
    void deposit(double amount);
    bool withdraw(double amount);
    double getBalance() const;
};

#endif
