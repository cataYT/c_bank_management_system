#pragma once

#include "account.h"

typedef struct bank bank;

void create_bank(const int starting_cash, const unsigned int capacity);
void add_account(account* acc);
account* search_account(const char* owner);
void deposit(account* acc, const unsigned int amount);
void withdraw(account* acc, const unsigned int amount);
void transfer_money(account* transferrer, account* transferee, const unsigned int amount);
void print_acc(const account* acc);
void print_bank();
void resize_bank(unsigned const int new_capacity);
void free_bank();