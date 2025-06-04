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

void transaction_log(const char* type, ...)
{
    va_list args;
    va_start(args, type);

    FILE* file = fopen("logs.txt", "a");
    if (file == NULL) 
    {
        perror("Failed to open file.\n");
        va_end(args);
        return;
    }

    if (strcmp(type, "add") == 0)
    {
        const char* name = va_arg(args, const char*);
        if (!name)
        {
            fprintf(stderr, "Null name in 'add'.\n");
        }
        else
        {
            fprintf(file, "Transaction: Added account %s.\n", name);
        }
    }
    else if (strcmp(type, "deposit") == 0)
    {
        const char* name = va_arg(args, const char*);
        const unsigned int amount = va_arg(args, const unsigned int);
        if (!name)
        {
            fprintf(stderr, "Null name in 'deposit'.\n");
        }
        else
        {
            fprintf(file, "Transaction: Deposited amount %d by %s.\n", amount, name);
        }
    }
    else if (strcmp(type, "withdraw") == 0)
    {
        const char* name = va_arg(args, const char*);
        const unsigned int amount = va_arg(args, const unsigned int);
        if (!name)
        {
            fprintf(stderr, "Null name in 'withdraw'.\n");
        }
        else
        {
            fprintf(file, "Transaction: Withdrew amount %d from %s.\n", amount, name);
        }
    }
    else if (strcmp(type, "transfer") == 0)
    {
        const char* from = va_arg(args, const char*);
        const char* to = va_arg(args, const char*);
        int amount = va_arg(args, int);

        if (!from || !to)
        {
            fprintf(stderr, "Null name(s) in 'transfer'.\n");
        }
        else
        {
            fprintf(file, "Transaction: Transferred amount %d from %s to %s.\n", amount, from, to);
        }
    }
    else
    {
        fprintf(stderr, "Unknown transaction type: %s\n", type);
    }

    fclose(file);
    va_end(args);
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

    transaction_log("add", owner);
}

void deposit(account* acc, const unsigned int deposit_amount)
{
    acc->balance += deposit_amount;
    BANK.cash += deposit_amount;

    const char* owner = acc->owner;

    printf("Deposited amount of %d to %s\n", deposit_amount, owner);
    transaction_log("deposit", owner, deposit_amount);
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

    const char* owner = acc->owner;

    printf("Withdrew amount of %d from %s\n", withdraw_amount, owner);
    transaction_log("withdraw", owner, withdraw_amount);
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

    const char* from_owner = from->owner;
    const char* to_owner = to->owner;

    printf("Money transferred from %s to %s of amount %d\n", from_owner, to_owner, amount);
    transaction_log("transfer", from_owner, to_owner, amount);
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

    /* FILE* file = fopen("logs.txt", "w");
    if (!file)
    {
        perror("failed to clear logs");
        exit(1);
    }

    fclose(file); */
}