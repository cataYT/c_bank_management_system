#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include "bank.h"
#include "../account/account.h"
#include "../account/account_codes.h"
#include "../vector/vector.h"

/**
 * @struct Bank struct to hold and manage accounts.
 * 
 */
struct bank {
    struct vector accs; /** Vector to hold the accounts. */
    int cash;           /** Current cash of the bank. */
    size_t size;        /** Current accounts in the bank. */
    size_t capacity;    /** Capacity of the bank to hold accounts. */
};

/**
 * @enum Enum for logging different transactions.
 * 
 */
enum transaction {
    Add,
    Deposit,
    Withdraw,
    Transfer
};

static struct bank BANK = {NULL, 0, 0, 0};
static int initialized = 0;

enum bank_error bank_initialize(const int starting_cash, const size_t capacity)
{
    if (capacity == 0)
    {
        fprintf(stderr, "capacity is 0 at create_bank()");
        return ERR_BANK_CAPACITY;
    }

    if (initialized && capacity != BANK.capacity)
    {
        fprintf(stderr, "bank already initialized with different capacity at create_bank()\n");
        return ERR_BANK_CAPACITY;
    }

    if (!initialized)
    {
        if (!vector_initialize(capacity, sizeof(struct account), &BANK.accs)) {
            return ERR_BANK_OPERATION_FAILED;
        }
        BANK.cash = starting_cash;
        BANK.size = 0;
        BANK.capacity = capacity;
        initialized = 1;
    }

    return OK_BANK;
}

/**
 * @brief Used internally by bank functions to log into logs.txt.
 * 
 * @param[in] type Type of transaction occuring.
 * @param[in] ... Used for getting accounts and amount.
 * @return OK_BANK if successful, ERR_BANK_OPERATION_FAILED otherwise.
 */
enum bank_error transaction_log(enum transaction type, ...)
{
    enum bank_error result = OK_BANK;

    if (type < 0 || type > Transfer) {
        fprintf(stderr, "invalid transaction type at transaction_log()\n");
        return ERR_BANK_OPERATION_FAILED;
    }

    va_list args;
    va_start(args, type);

    FILE *file = fopen("logs.txt", "a");
    if (file == NULL)
    {
        fprintf(stderr, "failed to open log file at transaction_log()\n");
        va_end(args);
        return ERR_BANK_OPERATION_FAILED;
    }

    switch (type) {
        case Add:
        {
            const char *name = va_arg(args, const char *);
            if (!name) {
                fprintf(stderr, "null name at transaction_log()\n");
                result = ERR_BANK_OPERATION_FAILED;
                goto cleanup;
            }
            fprintf(file, "Transaction: Added account %s.\n", name);
            break;
        }
        case Deposit:
        {
            const char *name = va_arg(args, const char *);
            const unsigned int amount = va_arg(args, const unsigned int);
            if (!name) {
                fprintf(stderr, "null name at transaction_log()\n");
                result = ERR_BANK_OPERATION_FAILED;
                goto cleanup;
            }
            fprintf(file, "Transaction: Deposited amount %u by %s.\n", amount, name);
            break;
        }
        case Withdraw:
        {
            const char *name = va_arg(args, const char *);
            const unsigned int amount = va_arg(args, const unsigned int);
            if (!name) {
                fprintf(stderr, "null name at transaction_log()\n");
                result = ERR_BANK_OPERATION_FAILED;
                goto cleanup;
            }
            fprintf(file, "Transaction: Withdrew amount %u from %s.\n", amount, name);
            break;
        }
        case Transfer:
        {
            const char *from = va_arg(args, const char *);
            const char *to = va_arg(args, const char *);
            const unsigned int amount = va_arg(args, const unsigned int);
            if (!from || !to) {
                fprintf(stderr, "null name(s) at transaction_log()\n");
                result = ERR_BANK_OPERATION_FAILED;
                goto cleanup;
            }
            fprintf(file, "Transaction: Transferred amount %u from %s to %s.\n", amount, from, to);
            break;
        }
        default:
        {
            fprintf(stderr, "unknown transaction type at transaction_log(): %d\n", type);
            result = ERR_BANK_OPERATION_FAILED;
            goto cleanup;
        }
    }

cleanup:
    if (file) {
        if (fclose(file) != 0) {
            fprintf(stderr, "failed to close file at transaction_log()\n");
            result = ERR_BANK_OPERATION_FAILED;
        }
    }
    file = NULL;
    va_end(args);
    return result;
}

bool bank_get_account(const char *owner, struct account *acc)
{
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *b_accs = BANK.accs.items;
        struct account *b_acc = &b_accs[i];
        if (strcmp(b_acc->owner, owner) == 0) {
            memcpy(acc, b_acc, sizeof(struct account));
            return true;
        }
    }
    *acc = (struct account){NULL};
    return false;
}

enum bank_error bank_add_account(const struct account *acc)
{
    if (!acc) {
        fprintf(stderr, "account is null at add_account()\n");
        return ERR_BANK_ACCOUNT_NULL;
    }

    struct account _placeholder = {NULL};
    if (bank_get_account(acc->owner, &_placeholder)) {
        printf("Account with the name %s already exists.\n", acc->owner);
        return ERR_BANK_ACCOUNT_EXISTS;
    }

    if (!vector_push_back(&BANK.accs, acc)) {
        return ERR_BANK_OPERATION_FAILED;
    }

    if (transaction_log(Add, acc->owner) != OK_BANK) {
        return ERR_BANK_OPERATION_FAILED;
    }

    BANK.size++;

    return OK_BANK;
}

enum bank_error bank_deposit(struct account *acc, const unsigned int deposit_amount)
{
    if (!acc) {
        fprintf(stderr, "account is null at deposit()\n");
        return ERR_BANK_ACCOUNT_NULL;
    }

    if (deposit_amount == 0) {
        fprintf(stderr, "amount is 0 at deposit()\n");
        return ERR_BANK_DEPOSIT;
    }

    acc->balance += deposit_amount;
    BANK.cash += deposit_amount;

    printf("Deposited amount of %d to %s\n", deposit_amount, acc->owner);
    if (transaction_log(Deposit, acc->owner, deposit_amount) != OK_BANK) {
        return ERR_BANK_OPERATION_FAILED;
    }

    return OK_BANK;
}

enum bank_error bank_withdraw(struct account *acc, const unsigned int withdraw_amount)
{
    if (!acc) {
        fprintf(stderr, "account is null at withdraw()\n");
        return ERR_BANK_ACCOUNT_NULL;
    }

    if (withdraw_amount == 0) {
        fprintf(stderr, "amount is 0 at withdraw()\n");
        return ERR_BANK_WITHDRAW;
    }

    if (acc->balance < withdraw_amount)
    {
        printf("withdrawal amount is greater than the balance at withdraw()\n");
        return ERR_BANK_WITHDRAW;
    }

    acc->balance -= withdraw_amount;
    BANK.cash -= withdraw_amount;

    printf("Withdrew amount of %d from %s\n", withdraw_amount, acc->owner);
    if (transaction_log(Withdraw, acc->owner, withdraw_amount) != OK_BANK) {
        return ERR_BANK_OPERATION_FAILED;
    }

    return OK_BANK;
}

void bank_print_bank()
{
    printf("Accounts: \n");
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *acc = &((struct account *)BANK.accs.items)[i];
        account_print_account(acc);
    }
    printf("Current cash: %d\n", BANK.cash);
    printf("Current size: %d\n", BANK.accs.size);
    printf("Current capacity: %d\n", BANK.accs.capacity);
}

enum bank_error bank_transfer_money(struct account *from, struct account *to, const unsigned int amount)
{
    if (!from) {
        fprintf(stderr, "from account is null at transfer_money()\n");
        return ERR_BANK_ACCOUNT_NULL;
    }

    if (!to) {
        fprintf(stderr, "to account is null at transfer_money()\n");
        return ERR_BANK_ACCOUNT_NULL;
    }

    if (amount == 0) {
        fprintf(stderr, "amount is 0 at transfer_money()\n");
        return ERR_BANK_AMOUNT;
    }
    
    if (from->balance < amount)
    {
        printf("transfer amount is greater than the balance at transfer_money()\n");
        return ERR_BANK_AMOUNT;
    }

    from->balance -= amount;
    to->balance += amount;

    printf("Money transferred from %s to %s of amount %d\n", from->owner, to->owner, amount);
    transaction_log(Transfer, from->owner, to->owner, amount);

    return OK_BANK;
}

enum bank_error bank_deinitialize()
{
    for (int i = 0; i < BANK.accs.size; i++) {
        struct account *accs = BANK.accs.items;
        struct account *acc = &accs[i];
        if (account_deinitialize(acc) != OK_ACCOUNT) {
            return ERR_BANK_ACCOUNT_NULL;
        }
        acc = NULL;
    }

    vector_deinitialize(&BANK.accs);

    BANK.size = 0;
    BANK.capacity = 0;
    BANK.cash = 0;

    return OK_BANK;

    /* FILE* file = fopen("logs.txt", "w");
    if (!file)
    {
        perror("failed to clear logs");
        exit(1);
    }

    fclose(file); */
}