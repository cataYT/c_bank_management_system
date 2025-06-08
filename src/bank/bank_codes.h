#pragma once

#define BANK_ERRORS                 \
    X(OK_BANK,                  0)  \
    X(ERR_BANK_OPERATION_FAILED, -7) \
    X(ERR_BANK_CAPACITY,       -6)  \
    X(ERR_BANK_ACCOUNT_NULL,   -5)  \
    X(ERR_BANK_ACCOUNT_EXISTS, -4)  \
    X(ERR_BANK_DEPOSIT,        -3)  \
    X(ERR_BANK_WITHDRAW,       -2)  \
    X(ERR_BANK_AMOUNT,         -1)