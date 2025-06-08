#include <stdio.h>
#include "account/account.h"
#include "bank/bank.h"

int main()
{
    enum bank_error bank_err = create_bank(1000, 2);
    if (bank_err != OK_BANK) {
        fprintf(stderr, "failed to create bank: %d\n", bank_err);
        return 1;
    }

    {
        struct account t_cata = {NULL};
        enum account_error acc_cata = create_account("Cata", 100, &t_cata);
        if (acc_cata != OK_ACCOUNT) {
            fprintf(stderr, "failed to create temp acc: %d\n", acc_cata);
            free_bank();
            return 1;
        }

        struct account t_nullify = {NULL};
        enum account_error acc_nullify = create_account("Nullify", 200, &t_nullify);
        if (acc_nullify != OK_ACCOUNT) {
            fprintf(stderr, "failed to create temp acc: %d\n", acc_nullify);
            free_bank();
            free_account(&t_cata);
            return 1;
        }

        enum bank_error bank_cata = add_account(&t_cata);
        if (bank_cata != OK_BANK) {
            fprintf(stderr, "failed to add account: %d\n", bank_cata);
            free_bank();
            free_account(&t_cata);
            free_account(&t_nullify);
            return 1;
        }

        enum bank_error bank_nullify = add_account(&t_nullify);
        if (bank_nullify != OK_BANK) {
            fprintf(stderr, "failed to add account: %d\n", bank_nullify);
            free_bank();
            free_account(&t_cata);
            free_account(&t_nullify);
            return 1;
        }
    }

    struct account cata = {NULL};
    if (!get_account("Cata", &cata)) {
        fprintf(stderr, "failed to create account\n");
        return 1;
    }

    struct account nullify = {NULL};
    if (!get_account("Nullify", &nullify)) {
        fprintf(stderr, "failed to create account\n");
        free_bank();
        return 1;
    }

    print_acc(&cata);
    print_acc(&nullify);

    enum bank_error deposit_err = deposit(&cata, 1000);
    if (deposit_err != OK_BANK) {
        fprintf(stderr, "failed to deposit to %s: %d\n", cata.owner, deposit_err);
        free_bank();
        return 1;
    }

    enum bank_error withdraw_err = withdraw(&cata, 100);
    if (withdraw_err != OK_BANK) {
        fprintf(stderr, "failed to withdraw from %s of amount %d", cata.owner, 100);
        free_bank();
        return 1;
    }

    print_acc(&cata);

    enum bank_error transfer_error = transfer_money(&cata, &nullify, 100);
    if (transfer_error != OK_BANK) {
        fprintf(stderr, "failed to transfer from %s to %s of amount %d", cata.owner, nullify.owner, 100);
        free_bank();
        return 1;
    }

    print_bank();

    enum bank_error free_error = free_bank();
    if (free_error != OK_BANK) {
        fprintf(stderr, "failed to free bank: %d\n", free_error);
        return 1;
    }

    return 0;
}