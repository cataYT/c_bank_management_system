#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include <stdbool.h>
#include <rpc.h>  // For UUID functions

struct account {
    char *owner;
    int balance;
    UUID _uuid; // DO NOT USE THIS! USE get_account_uuid()
};

bool create_account(const char *owner_name, const int starting_balance, struct account *acc);
const UUID get_account_uuid(const struct account *acc);
void print_acc(const struct account *acc);
bool check_null_account(struct account *acc);
bool free_account(struct account *acc);