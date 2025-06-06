#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include "bank.h"
#include "../account/account.h"
#include "../vector/vector.h"

struct bank
{
    struct vector accs;
    int cash;
    size_t size;
    size_t capacity;
};

static struct bank BANK = {NULL, 0, 0, 0};
static int initialized = 0;

const bool create_bank(const int starting_cash, const size_t capacity)
{
    if (capacity <= 0)
    {
        fprintf(stderr, "capacity cannot be 0 or negative");
        return false;
    }

    if (initialized && capacity != BANK.capacity)
    {
        fprintf(stderr, "Bank already initialized with different capacity.\n");
        return false;
    }

    if (!initialized)
    {
        BANK.accs = create_vector(capacity, sizeof(struct account));
        if (check_null_vector(&BANK.accs)) {
            return false;
        }
        BANK.cash = starting_cash;
        BANK.size = 0;
        BANK.capacity = capacity;
        initialized = 1;
    }

    return true;
}

const bool transaction_log(const char *type, ...)
{
    if (!type || strlen(type) == 0) {
        fprintf(stderr, "found type of transaction null in transaction_log()\n");
        return false;
    }

    va_list args;
    va_start(args, type);

    FILE *file = fopen("logs.txt", "a");
    if (file == NULL) 
    {
        fprintf(stderr, "Failed to open file.\n");
        va_end(args);
        return false;
    }

    if (strcmp(type, "add") == 0)
    {
        const char *name = va_arg(args, const char *);
        if (!name)
        {
            fprintf(stderr, "Null name in 'add'.\n");
            return false;
        }
        else
        {
            fprintf(file, "Transaction: Added account %s.\n", name);
            return true;
        }
    }
    else if (strcmp(type, "deposit") == 0)
    {
        const char *name = va_arg(args, const char *);
        const unsigned int amount = va_arg(args, const unsigned int);
        if (!name)
        {
            fprintf(stderr, "Null name in 'deposit'.\n");
            return false;
        }
        else
        {
            fprintf(file, "Transaction: Deposited amount %d by %s.\n", amount, name);
            return true;
        }
    }
    else if (strcmp(type, "withdraw") == 0)
    {
        const char *name = va_arg(args, const char *);
        const unsigned int amount = va_arg(args, const unsigned int);
        if (!name)
        {
            fprintf(stderr, "Null name in 'withdraw'.\n");
            return false;
        }
        else
        {
            fprintf(file, "Transaction: Withdrew amount %d from %s.\n", amount, name);
            return true;
        }
    }
    else if (strcmp(type, "transfer") == 0)
    {
        const char *from = va_arg(args, const char *);
        const char *to = va_arg(args, const char *);
        const unsigned int amount = va_arg(args, const unsigned int);

        if (!from || !to)
        {
            fprintf(stderr, "Null name(s) in 'transfer'.\n");
            return false;
        }
        else
        {
            fprintf(file, "Transaction: Transferred amount %d from %s to %s.\n", amount, from, to);
            return true;
        }
    }
    else
    {
        fprintf(stderr, "Unknown transaction type: %s\n", type);
        return false;
    }

    if (fclose(file) != 0)
    {
        fprintf(stderr, "failed to close file\n");
        return false;
    }
    file = NULL;
    va_end(args);
}

struct account *search_account(const char *owner)
{
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *accs = BANK.accs.items;
        struct account *acc = &accs[i];
        if (strcmp(acc->owner, owner) == 0) {
            return acc;
        }
    }
    return NULL;
}

// Takes ownership of account so it will invalidate previous pointer.
// Create them temporary and then access them from bank for valid account.
const bool add_account(const struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null at add_account()\n");
        return false;
    }

    if (search_account(acc->owner))
    {
        printf("Account with the name %s already exists.\n", acc->owner);
        return false;
    }

    if (!push_back(&BANK.accs, acc)) {
        return false;
    }

    if (!transaction_log("add", acc->owner)) {
        return false;
    }

    BANK.size++;

    return true;
}

const bool deposit(struct account *acc, const unsigned int deposit_amount)
{
    if (!acc) {
        fprintf(stderr, "account is null at deposit()\n");
        return false;
    }

    if (deposit_amount == 0) {
        fprintf(stderr, "amount is 0 at deposit()\n");
        return false;
    }

    acc->balance += deposit_amount;
    BANK.cash += deposit_amount;

    printf("Deposited amount of %d to %s\n", deposit_amount, acc->owner);
    if (!transaction_log("deposit", acc->owner, deposit_amount)) {
        return false;
    }

    return true;
}

const bool withdraw(struct account *acc, const unsigned int withdraw_amount)
{
    if (!acc) {
        fprintf(stderr, "account is null at withdraw()\n");
        return false;
    }

    if (withdraw_amount == 0) {
        fprintf(stderr, "amount is 0 at withdraw()\n");
        return false;
    }

    if (acc->balance < withdraw_amount)
    {
        printf("Withdrawal amount is greater than the balance!\n");
        return false;
    }

    acc->balance -= withdraw_amount;
    BANK.cash -= withdraw_amount;

    printf("Withdrew amount of %d from %s\n", withdraw_amount, acc->owner);
    if (!transaction_log("withdraw", acc->owner, withdraw_amount)) {
        return false;
    }

    return true;
}

void print_bank()
{
    printf("Accounts: \n");
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *acc = &((struct account *)BANK.accs.items)[i];
        print_acc(acc);
    }
    printf("Current cash: %d\n", BANK.cash);
    printf("Current size: %d\n", BANK.accs.size);
    printf("Current capacity: %d\n", BANK.accs.capacity);
}

const bool transfer_money(struct account *from, struct account *to, const unsigned int amount)
{
    if (!from) {
        fprintf(stderr, "from account is null at transfer_money()\n");
        return false;
    }

    if (!to) {
        fprintf(stderr, "to account is null at transfer_money()\n");
        return false;
    }

    if (amount == 0) {
        fprintf(stderr, "amount is 0 at transfer_money()\n");
        return false;
    }
    
    if (from->balance < amount)
    {
        printf("Transfer amount is greater than the balance in transferrer\n");
        return false;
    }

    from->balance -= amount;
    to->balance += amount;

    printf("Money transferred from %s to %s of amount %d\n", from->owner, to->owner, amount);
    transaction_log("transfer", from->owner, to->owner, amount);

    return true;
}

const bool free_bank()
{
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *accs = BANK.accs.items;
        struct account *acc = &accs[i];
        if (!free_account(acc)) {
            return false;
        }
        acc = NULL;
    }

    if (!free_vector(&BANK.accs)) {
        return false;
    }

    BANK.accs = NULL_VECTOR;

    BANK.size = 0;
    BANK.capacity = 0;
    BANK.cash = 0;

    return true;

    /* FILE* file = fopen("logs.txt", "w");
    if (!file)
    {
        perror("failed to clear logs");
        exit(1);
    }

    fclose(file); */
}