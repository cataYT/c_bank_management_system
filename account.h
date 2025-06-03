#pragma once

#include <stdlib.h>
#include <string.h>
#include <rpc.h>  // For UUID functions
#include <stdio.h>
#pragma comment(lib, "Rpcrt4.lib")

typedef struct account
{
    char* owner;
    int balance;
    UUID _uuid;
} account;

account create_account(const char* owner_name, const int starting_balance);
UUID get_account_uuid(const account* acc);
void free_account(account* acc);