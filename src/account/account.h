#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include <stdbool.h>
#include <rpc.h>  // For UUID functions

struct account {
    char *owner;
    int balance;
    UUID _uuid; // DO NOT USE THIS! USE get_account_uuid()
};

struct account create_account(const char *owner_name, const int starting_balance);
UUID get_account_uuid(const struct account *acc);
void print_acc(const struct account *acc);
const bool check_null_account(struct account *acc);
const bool free_account(struct account *acc);