#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include <rpc.h>  // For UUID functions

struct account
{
    char* owner;
    int balance;
    UUID _uuid; // DO NOT USE THIS! USE get_account_uuid()
};

struct account create_account(const char* owner_name, const int starting_balance);
UUID get_account_uuid(const struct account* acc);
void free_account(struct account* acc);