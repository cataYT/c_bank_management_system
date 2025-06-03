#include "bank.h"
#include "account.h"

bank* create_bank(const int starting_cash, const unsigned int capacity)
{
    static bank instance;
    static int initialized = 0;

    if (initialized && capacity != instance.capacity)
    {
        fprintf(stderr, "Bank already initialized with different capacity.\n");
        return &instance;
    }

    if (!initialized)
    {
        instance.accs = malloc(sizeof(account) * capacity);
        if (!instance.accs)
        {
            perror("malloc failed");
            exit(1);
        }
        instance.cash = starting_cash;
        instance.size = 0;
        instance.capacity = capacity;
        initialized = 1;
    }

    return &instance;
}

void add_account(bank* bank, const account* acc)
{
    if (bank->size >= bank->capacity)
    {
        printf("Capacity is full!\n");
        return;
    }

    if (search_account(bank, acc->owner))
    {
        printf("Account with the name %s already exists.\n", acc->owner);
        return;
    }

    account* dest = &bank->accs[bank->size];
    dest->balance = acc->balance;
    dest->uuid = acc->uuid;
    dest->owner = malloc(strlen(acc->owner) + 1);
    if (!dest->owner)
    {
        perror("malloc failed");
        exit(1);
    }
    strcpy(dest->owner, acc->owner);
    bank->size++;
}

void resize_bank(bank* bank, unsigned int new_capacity)
{
    if (new_capacity < bank->capacity)
    {
        printf("New capacity cannot be less than current capacity\n");
        return;
    }
    bank->capacity = new_capacity;

    account* new_accounts = realloc(bank->accs, sizeof(account) * new_capacity);
    if (!new_accounts)
    {
        perror("realloc failed, could not resize accounts");
        exit(1);
    }
    bank->accs = new_accounts;
    bank->capacity = new_capacity;

    printf("Bank capacity resized to %d\n", new_capacity);
}

void deposit(bank* bank, account* acc, unsigned int deposit_amount)
{
    acc->balance += deposit_amount;
    bank->cash += deposit_amount;

    printf("Deposited amount of %d to %s\n", deposit_amount, acc->owner);
}

void withdraw(bank* bank, account* acc, unsigned int withdraw_amount)
{
    if (acc->balance < withdraw_amount)
    {
        printf("Withdrawal amount is greater than the balance!\n");
        return;
    }

    acc->balance -= withdraw_amount;
    bank->cash -= withdraw_amount;

    printf("Withdrew amount of %d from %s\n", withdraw_amount, acc->owner);
}

void print_acc(account* acc)
{
    RPC_CSTR uuidStr = NULL;
    printf("%s : %d\n", acc->owner, acc->balance);
    if (UuidToStringA(&acc->uuid, &uuidStr) != RPC_S_OK)
    {
        fprintf(stderr, "Failed to convert UUID to string\n");
        return;
    }
    printf("UUID: %s\n", uuidStr);
    RpcStringFreeA(&uuidStr);
}

void print_bank(bank* bank)
{
    printf("Accounts: \n");
    for (int i = 0; i < bank->size; i++)
    {
        print_acc(&bank->accs[i]);
    }
    printf("Current cash: %d\n", bank->cash);
    printf("Current size: %d\n", bank->size);
    printf("Current capacity: %d\n", bank->capacity);
}

account* search_account(bank* bank, const char* owner)
{
    for (int i = 0; i < bank->size; i++)
    {
        if (strcmp(bank->accs[i].owner, owner) == 0)
        {
            return &bank->accs[i];
        }
    }
    return NULL;
}

void transfer_money(account* transferrer, account* transferree, unsigned int amount)
{
    if (transferrer->balance < amount)
    {
        printf("Transfer amount is greater than the balance in transferrer\n");
        return;
    }

    transferrer->balance -= amount;
    transferree->balance += amount;

    printf("Money transferred from %s to %s of amount %d\n", transferrer->owner, transferree->owner, amount);
}

void free_bank(bank* bank)
{
    if (!bank || !bank->accs)
    {
        return;
    }

    for (int i = 0; i < bank->size; i++)
    {
        free_account(&bank->accs[i]);
    }

    free(bank->accs);

    bank->accs = NULL;
    bank->size = 0;
    bank->capacity = 0;
    bank->cash = 0;
}