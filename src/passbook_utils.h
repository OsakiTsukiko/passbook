#ifndef PASSBOOK_UTILS_H
#define PASSBOOK_UTILS_H

#include <stdlib.h>
#include <vector.h> // part of my library (mystd) found on my github at https://github.com/OsakiTsukiko/mystd

// transaction types (enum)
typedef enum _TRANSACTION_TYPES {
    TRANSACTION_INCOME,
    TRANSACTION_EXPENSE
} TRANSACTION_TYPES;

// transaction struct
typedef struct _transaction
{   
    char* description;
    unsigned int date;
    TRANSACTION_TYPES type;
    unsigned int amount;
} transaction;

// transaction constructor
transaction* create_transaction(char *description, unsigned int date, TRANSACTION_TYPES type, unsigned int amount);
// transaction destroyer
void destroy_transaction(transaction *_transaction);

// interpret json array as transaction 
transaction* transaction_from_json(const char* json);
// interpret transaction id (a number) from a json array
int transaction_id_from_json(const char* json);
// stringifies a transaction vector into a json string (to be interpreted by the ui)
char* transaction_vector_to_json(vector* vec);

// custom strcat that works on dynamically allocated pointers as well (concatenates 2 strings)
char* cstm_strcat(char *dest, char *src);
// custom strncat that works on dynamically allocated pointers as well (concatenates 2 strings)
char* cstm_strncat(char *dest, char *src, int n);

// transaction vector wrappers 
// (vector uses void pointers so i made some wrappers for ease of use)
void tv_push_back(vector* vec, transaction* trans);
transaction* tv_get(vector* vec, unsigned int index);
transaction* tv_remove(vector* vec, unsigned int index);

#endif