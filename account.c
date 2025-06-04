#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc.h>
#include "account.h"

#define NULL_UUID (UUID){0}
#define NULL_ACCOUNT (struct account){NULL, 0, NULL_UUID}

void create_uuid(struct account* acc)
{
    if (!acc)
    {
        fprintf(stderr, "account is null, cannot create uuid\n");
        return;
    }
    RPC_STATUS status = UuidCreate(&acc->_uuid);
    // printf("UuidCreate status: %ld\n", status);

    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY)
    {
        fprintf(stderr, "Failed to create UUID (status: %ld)\n", status);
        return;
    }
}

struct account create_account(const char* owner_name, const int starting_balance)
{
    if (!owner_name || strlen(owner_name) == 0)
    {
        fprintf(stderr, "owner name is null, cannot create account\n");
        return NULL_ACCOUNT;
    }

    if (!starting_balance)
    {
        fprintf(stderr, "starting balance is null, cannot create account\n");
        return NULL_ACCOUNT;
    }

    struct account acc = NULL_ACCOUNT;
    acc.owner = malloc(strlen(owner_name) + 1);
    if (!acc.owner)
    {
        fprintf(stderr, "malloc failed");
        return NULL_ACCOUNT;
    }
    strcpy(acc.owner, owner_name);
    acc.balance = starting_balance;
    create_uuid(&acc);
    return acc;
}

UUID get_account_uuid(const struct account* acc)
{
    if (!acc)
    {
        fprintf(stderr, "account is null, cannot get uuid\n");
        return NULL_UUID;
    }
    return acc->_uuid;
}

void free_account(struct account* acc)
{
    if (!acc)
    {
        fprintf(stderr, "account is null, cannot free\n");
        return;
    }
    free(acc->owner);
    acc->owner = NULL;
    acc->balance = 0;
    memset(&acc->_uuid, 0, sizeof(acc->_uuid));
}