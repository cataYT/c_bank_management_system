#include <stdio.h>
#include "account/account.h"
#include "bank/bank.h"

int main()
{
    enum bank_error bank_err = bank_initialize(1000, 2);
    if (bank_err != OK_BANK) {
        fprintf(stderr, "failed to create bank: %d\n", bank_err);
        return 1;
    }

    {
        struct account t_cata = {NULL};
        enum account_error acc_cata = account_initialize("Cata", 100, &t_cata);
        if (acc_cata != OK_ACCOUNT) {
            fprintf(stderr, "failed to create temp acc: %d\n", acc_cata);
            bank_deinitialize();
            return 1;
        }

        struct account t_nullify = {NULL};
        enum account_error acc_nullify = account_initialize("Nullify", 200, &t_nullify);
        if (acc_nullify != OK_ACCOUNT) {
            fprintf(stderr, "failed to create temp acc: %d\n", acc_nullify);
            bank_deinitialize();
            account_deinitialize(&t_cata);
            return 1;
        }

        enum bank_error bank_cata = bank_add_account(&t_cata);
        if (bank_cata != OK_BANK) {
            fprintf(stderr, "failed to add account: %d\n", bank_cata);
            bank_deinitialize();
            account_deinitialize(&t_cata);
            account_deinitialize(&t_nullify);
            return 1;
        }

        enum bank_error bank_nullify = bank_add_account(&t_nullify);
        if (bank_nullify != OK_BANK) {
            fprintf(stderr, "failed to add account: %d\n", bank_nullify);
            bank_deinitialize();
            account_deinitialize(&t_cata);
            account_deinitialize(&t_nullify);
            return 1;
        }
    }

    struct account cata = {NULL};
    if (!bank_get_account("Cata", &cata)) {
        fprintf(stderr, "failed to create account\n");
        return 1;
    }

    struct account nullify = {NULL};
    if (!bank_get_account("Nullify", &nullify)) {
        fprintf(stderr, "failed to create account\n");
        bank_deinitialize();
        return 1;
    }

    account_print_account(&cata);
    account_print_account(&nullify);

    enum bank_error deposit_err = bank_deposit(&cata, 1000);
    if (deposit_err != OK_BANK) {
        fprintf(stderr, "failed to deposit to %s: %d\n", cata.owner, deposit_err);
        bank_deinitialize();
        return 1;
    }

    enum bank_error withdraw_err = bank_withdraw(&cata, 100);
    if (withdraw_err != OK_BANK) {
        fprintf(stderr, "failed to withdraw from %s of amount %d", cata.owner, 100);
        bank_deinitialize();
        return 1;
    }

    account_print_account(&cata);

    enum bank_error transfer_error = bank_transfer_money(&cata, &nullify, 100);
    if (transfer_error != OK_BANK) {
        fprintf(stderr, "failed to transfer from %s to %s of amount %d", cata.owner, nullify.owner, 100);
        bank_deinitialize();
        return 1;
    }

    bank_print_bank();

    enum bank_error free_error = bank_deinitialize();
    if (free_error != OK_BANK) {
        fprintf(stderr, "failed to free bank: %d\n", free_error);
        return 1;
    }

    return 0;
}