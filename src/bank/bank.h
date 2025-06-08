#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "../account/account.h"

bool create_bank(const int starting_cash, const size_t capacity);
bool add_account(const struct account *acc);
bool get_account(const char *owner, struct account *acc);
bool deposit(struct account *acc, const unsigned int amount);
bool withdraw(struct account *acc, const unsigned int amount);
bool transfer_money(struct account *from, struct account *to, const unsigned int amount);
void print_bank();
bool free_bank();