#pragma once

#include "account.h"

void create_bank(const int starting_cash, const unsigned int capacity);
void transaction_log(const char* type, ...);
void add_account(struct account* acc);
struct account* search_account(const char* owner);
void deposit(struct account* acc, const unsigned int amount);
void withdraw(struct account* acc, const unsigned int amount);
void transfer_money(struct account* from, struct account* to, const unsigned int amount);
void print_acc(const struct account* acc);
void print_bank();
void free_bank();