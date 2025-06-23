#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "../account/account.h"
#include "bank_codes.h"

/**
 * @enum Enum used for dispatching bank errors by macro.
 * 
 */
enum bank_error {
#define X(name, value) name = value,
    BANK_ERRORS
#undef X
};

/**
 * @brief Creates a singleton bank.
 * 
 * @param[in] starting_cash Starting cash for the bank.
 * @param[in] capacity Initial capacity for the bank to hold the accounts.
 * @return OK_BANK on success, ERR_BANK_CAPACITY or ERR_BANK_OPERATION_FAILED otherwise.
 */
enum bank_error bank_initialize(const int starting_cash, const size_t capacity);
/**
 * @brief Adds an account in the bank.
 * 
 * @param[in] acc Pointer to the account to add.
 * @return OK_BANK on success, ERR_BANK_ACCOUNT_NULL, ERR_BANK_ACCOUNT_EXISTS or ERR_BANK_OPERATION_FAILED otherwise.
 * @note After using this function, get the account by using get_account().
 */
enum bank_error bank_add_account(const struct account *acc);
/**
 * @brief Gets the account by its owner name.
 * 
 * @param[in] owner The owner name to get.
 * @param[out] acc Pointer for the account struct to hold the account.
 * @return true if account was found and copies it to the acc, false otherwise and sets the account struct to NULL.
 */
bool bank_get_account(const char *owner, struct account *acc);
/**
 * @brief Deposits the amount into the specified account. Increases the cash in bank by amount.
 * 
 * @param[in] acc Pointer to the account struct to deposit money into.
 * @param[in] amount Amount to deposit, cannot be 0.
 * @return OK_BANK if successful, ERR_BANK_ACCOUNT_NULL, ERR_BANK_DEPOSIT or ERR_BANK_OPERATION_FAILED otherwise.
 */
enum bank_error bank_deposit(struct account *acc, const unsigned int amount);
/**
 * @brief Withdraws the amount from the specfied account. Decreases the cash in bank by amount.
 * 
 * @param[in] acc Pointer to the account struct from which the money will be withdrawn.
 * @param[in] amount Amount to withdraw, cannot be 0.
 * @return OK_BANK if successful, ERR_BANK_ACCOUNT_NULL, ERR_BANK_WITHDRAW or ERR_BANK_OPERATION_FAILED otherwise.
 */
enum bank_error bank_withdraw(struct account *acc, const unsigned int amount);
/**
 * @brief Transfers money from one to another account.
 * 
 * @param[in] from Pointer to the account struct by which the money will be transferred.
 * @param[in] to Pointer to the account struct to which the money will be transferred.
 * @param[in] amount Amount to transfer, cannot be 0.
 * @return OK_BANK if successful, ERR_BANK_ACCOUNT_NULL or ERR_BANK_AMOUNT otherwise.
 */
enum bank_error bank_transfer_money(struct account *from, struct account *to, const unsigned int amount);
/**
 * @brief Prints the bank details.
 * 
 */
void bank_print_bank();
/**
 * @brief Deinitializes the bank struct.
 * 
 * @return OK_BANK if successful, ERR_BANK_ACCOUNT_NULL or ERR_BANK_OPERATION_FAILED otherwise.
 */
enum bank_error bank_deinitialize();