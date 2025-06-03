#include "account.h"

void create_uuid(account* acc)
{
    RPC_STATUS status = UuidCreate(&acc->uuid);
    printf("UuidCreate status: %ld\n", status);

    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY)
    {
        fprintf(stderr, "Failed to create UUID (status: %ld)\n", status);
        exit(1);
    }
}

account create_account(const char* owner_name, int starting_balance)
{
    account acc = {NULL, 0};
    acc.owner = malloc(strlen(owner_name) + 1);
    if (!acc.owner)
    {
        perror("malloc failed");
        exit(1);
    }
    strcpy(acc.owner, owner_name);
    acc.balance = starting_balance;
    create_uuid(&acc);
    return acc;
}

void free_account(account* acc)
{
    if (acc)
    {
        free(acc->owner);
        acc->owner = NULL;
    }
}
