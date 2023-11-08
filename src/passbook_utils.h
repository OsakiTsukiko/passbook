#ifndef PASSBOOK_UTILS_H
#define PASSBOOK_UTILS_H

#include <stdlib.h>
#include <vector.h>

typedef enum _TRANSACTION_TYPES {
    TRANSACTION_INCOME,
    TRANSACTION_EXPENSE
} TRANSACTION_TYPES;

typedef struct _transaction
{   
    char* description;
    unsigned int date;
    TRANSACTION_TYPES type;
    unsigned int amount;
} transaction;

transaction* create_transaction(char *description, unsigned int date, TRANSACTION_TYPES type, unsigned int amount);
void destroy_transaction(transaction *_transaction);

transaction* transaction_from_json(const char* json);

char* cstm_strcat(char *dest, char *src);
char* transaction_vector_to_json(vector* vec);

// transaction vector wrappers
void tv_push_back(vector* vec, transaction* trans);

transaction* tv_get(vector* vec, unsigned int index);

#endif