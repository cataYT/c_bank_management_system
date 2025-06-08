#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include <stdbool.h>
#include <rpc.h>  // For UUID functions
#include "account_codes.h"

struct account {
    char *owner;
    int balance;
    UUID _uuid; // DO NOT USE THIS! USE get_account_uuid()
};

enum account_error {
#define X(name, value) name = value,
    ACCOUNT_ERRORS
#undef X
};

enum account_error create_account(const char *owner_name, const int starting_balance, struct account *acc);
const UUID get_account_uuid(const struct account *acc);
enum account_error print_acc(const struct account *acc);
bool check_null_account(struct account *acc);
enum account_error free_account(struct account *acc);