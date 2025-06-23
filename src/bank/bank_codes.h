#pragma once

/**
 * @brief Macro defination of bank error codes.
 * 
 */
#define BANK_ERRORS                                                                                                                                     \
    X(OK_BANK,                    0)   /** Used if operations related to bank were successful. */                                                       \
    X(ERR_BANK_OPERATION_FAILED, -7)   /** Used if bank operation failed due to unexpected errors. */                                                   \
    X(ERR_BANK_CAPACITY,         -6)   /** Used if bank was created with 0 capacity or initializing it with different capacity in bank_initialize(). */ \
    X(ERR_BANK_ACCOUNT_NULL,     -5)   /** Used if account was passed as null in bank function arguements. */                                           \
    X(ERR_BANK_ACCOUNT_EXISTS,   -4)   /** Used if account already exists in add_account(). */                                                          \
    X(ERR_BANK_DEPOSIT,          -3)   /** Used if deposit amount is 0 in deposit(). */                                                                 \
    X(ERR_BANK_WITHDRAW,         -2)   /** Used if withdraw amount is 0 in withdraw() */                                                                \
    X(ERR_BANK_AMOUNT,           -1)   /** Used if amount is 0 or transfer amount is greater than the balance at bank_transfer_money() */