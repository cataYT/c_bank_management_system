#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "../account/account.h"

const bool create_bank(const int starting_cash, const size_t capacity);
const bool transaction_log(const char *type, ...);
const bool add_account(const struct account *acc);
struct account *search_account(const char *owner);
const bool deposit(struct account *acc, const unsigned int amount);
const bool withdraw(struct account *acc, const unsigned int amount);
const bool transfer_money(struct account *from, struct account *to, const unsigned int amount);
void print_bank();
const bool free_bank();