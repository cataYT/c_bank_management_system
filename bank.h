#ifndef BANK_H
#define BANK_H

#include "account.h"

typedef struct bank
{
    account* accs;
    int cash;
    int size;
    int capacity;
} bank;

// Function declarations using the opaque type `bank*`
bank* create_bank(int starting_cash, unsigned int capacity);
void add_account(bank* bank, const account* acc);
account* search_account(bank* bank, const char* owner);
void deposit(bank* bank, account* acc, unsigned int amount);
void withdraw(bank* bank, account* acc, unsigned int amount);
void transfer_money(account* transferrer, account* transferee, unsigned int amount);
void print_acc(account* acc);
void print_bank(bank* bank);
void resize_bank(bank* bank, unsigned int new_capacity);
void free_bank(bank* bank);

#endif // BANK_H
