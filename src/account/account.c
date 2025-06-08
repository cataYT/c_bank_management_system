#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc.h>
#include "account.h"
#include "account_codes.h"

enum account_error create_uuid(struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null at create_uuid()\n");
        return ERR_ACCOUNT_NULL;
    }
    RPC_STATUS status = UuidCreate(&acc->_uuid);
    // printf("UuidCreate status: %ld\n", status);

    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY) {
        fprintf(stderr, "Failed to create UUID (status: %ld)\n", status);
        return ERR_ACCOUNT_UUID;
    }

    return OK_ACCOUNT_UUID;
}

enum account_error create_account(const char *owner_name, const int starting_balance, struct account *acc)
{
    if (!owner_name || strlen(owner_name) == 0) {
        fprintf(stderr, "owner name is null at create_account()\n");
        return ERR_ACCOUNT_OWNER;
    }

    if (!starting_balance) {
        fprintf(stderr, "starting balance is null at create_account()\n");
        return ERR_ACCOUNT_BALANCE;
    }

    acc->owner = malloc(strlen(owner_name) + 1);
    if (!acc->owner) {
        fprintf(stderr, "malloc failed at create_account()\n");
        return ERR_MALLOC_ACCOUNT;
    }
    strcpy(acc->owner, owner_name);
    acc->balance = starting_balance;
    if (create_uuid(acc) != OK_ACCOUNT_UUID) {
        return ERR_ACCOUNT_UUID;
    }
    return OK_ACCOUNT;
}

const UUID get_account_uuid(const struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null at get_account_uuid()\n");
        return (UUID){0};
    }
    return acc->_uuid;
}

enum account_error print_acc(const struct account *acc)
{
    RPC_CSTR uuidStr = NULL;
    printf("%s : %d\n", acc->owner, acc->balance);
    UUID t_uuid = get_account_uuid(acc);
    if (UuidToStringA(&t_uuid, &uuidStr) != RPC_S_OK)
    {
        fprintf(stderr, "failed to convert uuid to string at get_account_uuid()\n");
        return ERR_ACCOUNT_UUID;
    }
    printf("UUID: %s\n", uuidStr);
    RPC_STATUS status = RpcStringFreeA(&uuidStr);
    if (status != RPC_S_OK)
    {
        fprintf(stderr, "failed to free RPC string: %lu\n", status);
        return ERR_ACCOUNT_UUID;
    }
    uuidStr = NULL;
    return OK_ACCOUNT;
}

bool check_null_account(struct account *acc)
{
    RPC_STATUS temp_status = 0;
    if (!acc->owner || UuidIsNil(&acc->_uuid, &temp_status)) {
        return true;
    }

    return false;
}

enum account_error free_account(struct account *acc)
{
    if (!acc) {
        return ERR_ACCOUNT_NULL;
    }

    free(acc->owner);
    acc->owner = NULL;
    acc->balance = 0;
    memset(&acc->_uuid, 0, sizeof(acc->_uuid));

    return OK_ACCOUNT;
}