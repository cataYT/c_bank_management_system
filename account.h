#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdlib.h>
#include <string.h>
#include <rpc.h>  // For UUID functions
#include <stdio.h>
#pragma comment(lib, "Rpcrt4.lib")

typedef struct account
{
    char* owner;
    int balance;
    UUID uuid;
} account;

// Function declarations
void create_uuid(account* acc);
account create_account(const char* owner_name, int starting_balance);
void free_account(account* acc);

#endif // ACCOUNT_H
