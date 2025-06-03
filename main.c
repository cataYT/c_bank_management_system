#include "bank.h"

int main()
{
    create_bank(1000, 2);

    account t_cata = create_account("Cata", 100);
    account t_nullify = create_account("Nullify", 200);

    add_account(&t_cata);
    add_account(&t_nullify);

    account* cata = search_account("Cata");
    account* nullify = search_account("Nullify");

    print_acc(cata);
    print_acc(nullify);

    deposit(cata, 1000);
    withdraw(cata, 100);

    print_acc(cata);

    transfer_money(cata, nullify, 100);

    print_bank();

    free_bank();

    return 0;
}