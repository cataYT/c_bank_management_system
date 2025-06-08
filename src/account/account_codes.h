#pragma once

#define ACCOUNT_ERRORS            \
    X(OK_ACCOUNT,          0)     \
    X(OK_ACCOUNT_UUID,     0)     \
    X(ERR_MALLOC_ACCOUNT,          -1337) \
    X(ERR_ACCOUNT_NULL,    -4)    \
    X(ERR_ACCOUNT_OWNER,   -3)    \
    X(ERR_ACCOUNT_BALANCE, -2)    \
    X(ERR_ACCOUNT_UUID,    -1)