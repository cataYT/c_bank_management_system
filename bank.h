#pragma once

#include "account.h"
#include <stdarg.h>

typedef struct bank bank;

void create_bank(const int starting_cash, const unsigned int capacity);
void transaction_log(const char* type, ...);
void add_account(account* acc);
account* search_account(const char* owner);
void deposit(account* acc, const unsigned int amount);
void withdraw(account* acc, const unsigned int amount);
void transfer_money(account* from, account* to, const unsigned int amount);
void print_acc(const account* acc);
void print_bank();
void free_bank();