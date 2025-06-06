#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc.h>
#include "account.h"

#define NULL_UUID (UUID){0}
#define NULL_ACCOUNT (struct account){NULL, 0, NULL_UUID}

const bool create_uuid(struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null, cannot create uuid\n");
        return false;
    }
    RPC_STATUS status = UuidCreate(&acc->_uuid);
    // printf("UuidCreate status: %ld\n", status);

    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY) {
        fprintf(stderr, "Failed to create UUID (status: %ld)\n", status);
        return false;
    }

    return true;
}

struct account create_account(const char *owner_name, const int starting_balance)
{
    if (!owner_name || strlen(owner_name) == 0) {
        fprintf(stderr, "owner name is null, cannot create account\n");
        return NULL_ACCOUNT;
    }

    if (!starting_balance) {
        fprintf(stderr, "starting balance is null, cannot create account\n");
        return NULL_ACCOUNT;
    }

    struct account acc = NULL_ACCOUNT;
    acc.owner = malloc(strlen(owner_name) + 1);
    if (!acc.owner) {
        fprintf(stderr, "malloc failed");
        return NULL_ACCOUNT;
    }
    strcpy(acc.owner, owner_name);
    acc.balance = starting_balance;
    if (!create_uuid(&acc)) {
        return NULL_ACCOUNT;
    }
    return acc;
}

UUID get_account_uuid(const struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null, cannot get uuid\n");
        return NULL_UUID;
    }
    return acc->_uuid;
}

void print_acc(const struct account *acc)
{
    RPC_CSTR uuidStr = NULL;
    printf("%s : %d\n", acc->owner, acc->balance);
    UUID t_uuid = get_account_uuid(acc);
    if (UuidToStringA(&t_uuid, &uuidStr) != RPC_S_OK)
    {
        fprintf(stderr, "Failed to convert UUID to string\n");
        return;
    }
    printf("UUID: %s\n", uuidStr);
    RPC_STATUS status = RpcStringFreeA(&uuidStr);
    if (status != RPC_S_OK)
    {
        fprintf(stderr, "Failed to free RPC string: %lu\n", status);
    }
    uuidStr = NULL;
}

const bool check_null_account(struct account *acc) {
    RPC_STATUS temp_status = 0;
    if (!acc->owner || UuidIsNil(&acc->_uuid, &temp_status)) {
        return true;
    }

    return false;
}

const bool free_account(struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null, cannot free\n");
        return false;
    }

    free(acc->owner);
    acc->owner = NULL;
    acc->balance = 0;
    memset(&acc->_uuid, 0, sizeof(acc->_uuid));

    return true;
}