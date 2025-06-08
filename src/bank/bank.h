#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "../account/account.h"
#include "bank_codes.h"

enum bank_error {
#define X(name, value) name = value,
    BANK_ERRORS
#undef X
};

enum bank_error create_bank(const int starting_cash, const size_t capacity);
enum bank_error add_account(const struct account *acc);
bool get_account(const char *owner, struct account *acc);
enum bank_error deposit(struct account *acc, const unsigned int amount);
enum bank_error withdraw(struct account *acc, const unsigned int amount);
enum bank_error transfer_money(struct account *from, struct account *to, const unsigned int amount);
void print_bank();
enum bank_error free_bank();