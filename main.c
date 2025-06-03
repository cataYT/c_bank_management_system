#include "bank.h"

int main()
{
    bank* bank = create_bank(1000, 2);

    account t_cata = create_account("Cata", 100);
    account t_nullify = create_account("Nullify", 200);

    add_account(bank, &t_cata);
    add_account(bank, &t_nullify);

    free_account(&t_cata);
    free_account(&t_nullify);

    account* cata = search_account(bank, "Cata");
    account* nullify = search_account(bank, "Nullify");

    print_acc(cata);
    print_acc(nullify);

    deposit(bank, cata, 1000);
    withdraw(bank, cata, 100);

    print_acc(cata);
    printf("Bank cash: %d\n", bank->cash);

    transfer_money(cata, nullify, 100);

    print_bank(bank);

    free_bank(bank);
    
    return 0;
}