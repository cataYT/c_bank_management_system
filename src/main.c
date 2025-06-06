#include <stdio.h>
#include "account/account.h"
#include "bank/bank.h"

int main()
{
    if (!create_bank(1000, 2)) {
        fprintf(stderr, "failed to create bank\n");
        return 1;
    }

    {
        struct account t_cata = create_account("Cata", 100);
        if (check_null_account(&t_cata)) {
            fprintf(stderr, "failed to create temp acc\n");
            free_bank();
            return 1;
        }
        struct account t_nullify = create_account("Nullify", 200);
        if (check_null_account(&t_nullify)) {
            fprintf(stderr, "failed to create temp acc\n");
            free_bank();
            free_account(&t_cata);
            return 1;
        }

        if (!add_account(&t_cata)) {
            fprintf(stderr, "failed to add account\n");
            free_bank();
            free_account(&t_cata);
            free_account(&t_nullify);
            return 1;
        }

        if (!add_account(&t_nullify)) {
            fprintf(stderr, "failed to add account\n");
            free_bank();
            free_account(&t_cata);
            free_account(&t_nullify);
            return 1;
        }
    }

    struct account *cata = search_account("Cata");
    if (!cata) {
        fprintf(stderr, "failed to create account\n");
        return 1;
    }

    struct account *nullify = search_account("Nullify");
    if (!nullify) {
        fprintf(stderr, "failed to create account\n");
        free_bank();
        return 1;
    }

    print_acc(cata);
    print_acc(nullify);

    if (!deposit(cata, 1000)) {
        fprintf(stderr, "failed to deposit to %s of amount %d", cata->owner, 1000);
        free_bank();
        return 1;
    }

    if (!withdraw(cata, 100)) {
        fprintf(stderr, "failed to withdraw from %s of amount %d", cata->owner, 100);
        free_bank();
        return 1;
    }

    print_acc(cata);

    if (!transfer_money(cata, nullify, 100)) {
        fprintf(stderr, "failed to transfer from %s to %s of amount %d", cata->owner, nullify->owner, 100);
        free_bank();
        return 1;
    }

    print_bank();

    free_bank();

    return 0;
}