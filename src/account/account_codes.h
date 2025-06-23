#pragma once

/**
 * @def ACCOUNT_ERRORS
 * @brief Used for errors associated with accounts.
 */
#define ACCOUNT_ERRORS                                                                                      \
    X(OK_ACCOUNT,           0)    /** Used if account related functions are successful */                   \
    X(OK_ACCOUNT_UUID,      0)    /** Used if creating uuid was successful. */                              \
    X(ERR_MALLOC_ACCOUNT,  -1337) /** Used if malloc failed in allocating owner name. */                    \
    X(ERR_ACCOUNT_NULL,    -4)    /** Used if account is passed as null in account functions. */            \
    X(ERR_ACCOUNT_OWNER,   -3)    /** Used if account owner name is passed as null in account functions. */ \
    X(ERR_ACCOUNT_UUID,    -1)    /** Used if failed to create account uuid in account_initialize() */