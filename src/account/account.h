#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include <stdbool.h>
#include <rpc.h>  // For UUID functions
#include "account_codes.h"

/**
 * @struct Struct representing an account in the bank.
 */
struct account {
    char *owner; /** Name of the account holder. */
    int balance; /** Balance of the account. */
    UUID _uuid;  /** UUID of the account. @warning Should not be used directly, use get_account_uuid() instead. */
};

/**
 * @enum Enum for dispatching account errors by macro.
 */
enum account_error {
#define X(name, value) name = value,
    ACCOUNT_ERRORS
#undef X
};

/**
 * @brief Creates an account. Use bank_add_account() to register it.
 * 
 * @param[in] owner_name The name to be created under. Will throw ERR_ACCOUNT_OWNER if this is NULL.
 * @param[in] starting_balance Starting balance of the account.
 * @param[in,out] acc Pointer to the account struct, should be allocated by the caller.
 * @return OK_ACCOUNT on success, ERR_ACCOUNT_OWNER, ERR_ACCOUNT_UUID or ERR_MALLOC_ACCOUNT otherwise.
 */
enum account_error account_initialize(const char *owner_name, const int starting_balance, struct account *acc);
/**
 * @brief Get the associated uuid of the account.
 * 
 * @param[in] acc Pointer to the account struct.
 * @return UUID if successful, (UUID)(0) if otherwise.
 */
const UUID account_get_uuid(const struct account *acc);
/**
 * @brief Prints all the account details.
 * 
 * @param[in] acc Pointer to the account struct.
 * @return OK_ACCOUNT if success, ERR_ACCOUNT_UUID otherwise.
 */
enum account_error account_print_account(const struct account *acc);
/**
 * @brief Checks if the account is null.
 * 
 * @param[in] acc Pointer to the account struct.
 * @return true if account is null, false otherwise.
 */
bool account_check_null(struct account *acc);
/**
 * @brief Frees the account struct.
 * 
 * @param[in] acc Pointer to the account struct.
 * @return OK_ACCOUNT on successful, ERR_ACCOUNT_NULL otherwise.
 */
enum account_error account_deinitialize(struct account *acc);