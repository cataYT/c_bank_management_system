#include "bank.h"
#include "account.h"
#include "vector.h"

typedef struct bank
{
    vector accs;
    int cash;
    int size;
    int capacity;
} bank;

static bank BANK = {NULL, 0, 0, 0};
static int initialized = 0;

void create_bank(const int starting_cash, const unsigned int capacity)
{
    if (initialized && capacity != BANK.capacity)
    {
        fprintf(stderr, "Bank already initialized with different capacity.\n");
    }

    if (!initialized)
    {
        BANK.accs = create_vector(capacity, sizeof(account));
        BANK.cash = starting_cash;
        BANK.size = 0;
        BANK.capacity = capacity;
        initialized = 1;
    }
}

// Takes ownership of account so it will invalidate previous pointer.
// Create them temporary and then access them from bank for valid account.
void add_account(account* acc)
{
    const char* owner = acc->owner;

    if (search_account(owner))
    {
        printf("Account with the name %s already exists.\n", owner);
        return;
    }

    push_back(&BANK.accs, acc);
    BANK.size++;
}

void deposit(account* acc, const unsigned int deposit_amount)
{
    acc->balance += deposit_amount;
    BANK.cash += deposit_amount;

    printf("Deposited amount of %d to %s\n", deposit_amount, acc->owner);
}

void withdraw(account* acc, const unsigned int withdraw_amount)
{
    if (acc->balance < withdraw_amount)
    {
        printf("Withdrawal amount is greater than the balance!\n");
        return;
    }

    acc->balance -= withdraw_amount;
    BANK.cash -= withdraw_amount;

    printf("Withdrew amount of %d from %s\n", withdraw_amount, acc->owner);
}

void print_acc(const account* acc)
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
    RpcStringFreeA(&uuidStr);
}

void print_bank()
{
    printf("Accounts: \n");
    for (int i = 0; i < BANK.accs.size; i++)
    {
        account* acc = &((account*)BANK.accs.items)[i];
        print_acc(acc);
    }
    printf("Current cash: %d\n", BANK.cash);
    printf("Current size: %d\n", BANK.accs.size);
    printf("Current capacity: %d\n", BANK.accs.capacity);
}

account* search_account(const char* owner)
{
    for (int i = 0; i < BANK.accs.size; i++)
    {
        account* accs = BANK.accs.items;
        account* acc = &accs[i];
        if (strcmp(acc->owner, owner) == 0)
        {
            return acc;
        }
    }
    return NULL;
}

void transfer_money(account* from, account* to, const unsigned int amount)
{
    if (from->balance < amount)
    {
        printf("Transfer amount is greater than the balance in transferrer\n");
        return;
    }

    from->balance -= amount;
    to->balance += amount;

    printf("Money transferred from %s to %s of amount %d\n", from->owner, to->owner, amount);
}

void free_bank()
{
    for (int i = 0; i < BANK.accs.size; i++)
    {
        account* accs = BANK.accs.items;
        account* acc = &accs[i];
        free_account(acc);
    }

    free_vector(&BANK.accs);

    BANK.size = 0;
    BANK.capacity = 0;
    BANK.cash = 0;
}