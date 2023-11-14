#include <string.h>
#include "passbook_utils.h"
#include <stdio.h>

transaction* create_transaction(char *description, unsigned int date, TRANSACTION_TYPES type, unsigned int amount) {
    transaction *res = (transaction *)(malloc(sizeof(transaction)));
    res->description = (char *)malloc(sizeof(char) * strlen(description) + sizeof(description));
    memcpy(res->description, description, strlen(description));
    res->description[strlen(description)] = '\0';
    
    res->date = date;
    res->type = type;
    res->amount = amount;
    return res;
}

void destroy_transaction(transaction *_transaction) {
    free(_transaction->description);
    free(_transaction);
}

void tv_push_back(vector* vec, transaction* trans) {
    vector_push_back(vec, trans);
}

void tv_push_front(vector* vec, transaction* trans) {
    vector_push_front(vec, trans);
}

transaction* tv_pop_back(vector* vec) {
    return (transaction *)vector_pop_back(vec);
}

transaction* tv_pop_front(vector* vec) {
    return (transaction *)vector_pop_front(vec);
}

transaction* tv_remove(vector* vec, unsigned int index) {
    return (transaction *)vector_remove(vec, index);
}

transaction* tv_get(vector* vec, unsigned int index) {
    return (transaction *)vec->array[index];
}

char* cstm_strcat(char *dest, char *src) {
    char* result = (char *)malloc(
        sizeof(char) * (
            strlen(dest) +
            strlen(src)
            + 1
        )
    );
    memcpy(result, dest, sizeof(char) * strlen(dest));
    memcpy(result + strlen(dest), src, sizeof(char) * strlen(src) + sizeof(char));
    return result;
}

char* cstm_strncat(char *dest, char *src, int n) {
    char* result = (char *)malloc(
        sizeof(char) * (
            strlen(dest) +
            strlen(src)
            + 1
            + n
        )
    );
    memcpy(result, dest, sizeof(char) * strlen(dest));
    memcpy(result + strlen(dest) + n, src, sizeof(char) * strlen(src) + sizeof(char));
    return result;
}

char* int_to_str(int number)
{
    if (number == 0) {
        char* res = (char *)malloc(sizeof(char) * 2);
        res[0] = '0';
        res[1] = 0;
        return res;
    }
    int a = number;
    int n = 0;
    while (a > 0) {
        n += 1;
        a /= 10;
    }
    
    char* res = (char *)malloc(sizeof(char) * (n + 1));
    res[n] = 0;
    while (number > 0) {
        int d = number % 10;
        number /= 10;
        res[n-1] = '0' + d;
        n -= 1;
    }

    return res;
}

transaction* transaction_from_json(const char* json) {
    int i = 1;
    // [1674000000000,"BRUH","expense","50"] example
    
    int date = 0;
    while (json[i] != ',') {
        // printf("%c, %d, %d\n", json[i], json[i], json[i] - '0');
        date *= 10;
        date += json[i] - '0';
        i += 1;
    }
    
    i += 2;
    char* desc = (char *)malloc(sizeof(char));
    char* l = (char *)malloc(sizeof(char) * 2);
    l[1] = '\0';
    desc[0] = '\0';
    while (json[i] != '"') {
        l[0] = json[i];
        char* h = cstm_strcat(desc, l);
        free(desc);
        desc = h;
        i += 1;
    }

    i += 3;
    char* type = (char *)malloc(sizeof(char));
    l[1] = '\0';
    type[0] = '\0';
    while (json[i] != '"') {
        l[0] = json[i];
        char* h = cstm_strcat(type, l);
        free(type);
        type = h;
        i += 1;
    }
    free(l);

    TRANSACTION_TYPES t;
    if (type[0] == 'i') {
        t = TRANSACTION_INCOME;
    } else {
        t = TRANSACTION_EXPENSE;
    }
    free(type);

    i += 2;

    int value = 0;
    while (json[i] != ']') {
        value *= 10;
        value += json[i] - '0';
        i += 1;
    }

    transaction* bruh = create_transaction(
        desc,
        (unsigned int)date,
        t,
        (unsigned int)value
    );

    free(desc);
    return bruh;
}

// really, should be unsigned, but im lazy rn...
int transaction_id_from_json(const char* json) {
    /*
    Programming on the train rn.. not fun
    waited for the train 3h.. must love cfr
    */
    int i = 1;
    // [5] example
    
    int id = 0;
    while (json[i] != ']') {
        id *= 10;
        id += json[i] - '0';
        i += 1;
    }

    return id;
}

// really, should be unsigned, but im lazy rn...
vector* transaction_vec_from_dates(const char* json, vector* vec) {
    vector* res = new_vector();
    int i = 1;

    int d1 = 0;
    while (json[i] != ',') {
        d1 *= 10;
        d1 += json[i] - '0';
        i += 1;
    }
    i += 1;
    int d2 = 0;
    while (json[i] != ']') {
        d2 *= 10;
        d2 += json[i] - '0';
        i += 1;
    }

    for (int i = 0; i < vector_size(vec); i += 1) {
        transaction* t = tv_get(vec, i);
        if (t->date >= d1 && t->date <= d2) {
            tv_push_back(res, t);
        }
    }
    
    return res;
}

char* transaction_vector_to_json(vector* vec) {
    char* res = (char *)malloc(sizeof(char));
    res[0] = '\0';
    char* holder;
    holder = cstm_strcat(res, "[");
    free(res);
    res = holder;
    for (int i = 0; i < vector_size(vec); i += 1) {
        transaction* trans = tv_get(vec, i);

        holder = cstm_strcat(res, "{ ");
        free(res);
        res = holder;

        holder = cstm_strcat(res, "\"description\":\"");
        free(res);
        res = holder;

        holder = cstm_strcat(res, trans->description);
        free(res);
        res = holder;

        holder = cstm_strcat(res, "\", ");
        free(res);
        res = holder;

        holder = cstm_strcat(res, "\"date\":\"");
        free(res);
        res = holder;

        char* date = int_to_str(trans->date);
        holder = cstm_strcat(res, date);
        free(res);
        res = holder;
        free(date);

        holder = cstm_strcat(res, "\", ");
        free(res);
        res = holder;

        holder = cstm_strcat(res, "\"type\":\"");
        free(res);
        res = holder;

        if (trans->type == TRANSACTION_EXPENSE) {
            holder = cstm_strcat(res, "expense");  
            free(res);
            res = holder;  

        } else {
            holder = cstm_strcat(res, "income"); 
            free(res);
            res = holder;

        }

        holder = cstm_strcat(res, "\", ");
        free(res);
        res = holder;

        holder = cstm_strcat(res, "\"value\":\"");
        free(res);
        res = holder;

        char* amount = int_to_str(trans->amount);
        holder = cstm_strcat(res, amount);
        free(res);
        res = holder;
        free(amount);
        
        holder = cstm_strcat(res, "\" }, ");
        free(res);
        res = holder;
    }
    holder = cstm_strncat(res, "]", -2);
    free(res);
    res = holder;
    return res;
}

void dump_transaction_vec(char* filename, vector* vec) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    
    for (int i = 0; i < vector_size(vec); i += 1) {
        transaction* t = tv_get(vec, i);
        fprintf(fptr, "%d %d %d %s\n", t->date, t->type, t->amount, t->description);
    }

    fclose(fptr); 
}

void load_transaction_vec(char* filename, vector* vec) {
    FILE *fptr;
    fptr = fopen(filename, "r");
    
    int d;
    int t;
    int a;
    char desc[512];

    while (fscanf(fptr, "%d %d %d", &d, &t, &a) == 3) {
        fgets(desc, 512, fptr);
        desc[strlen(desc)-1] = '\0';
        printf("%d %d %d %s\n", d, t, a, desc);
        transaction* tr = create_transaction(desc, d, t, a);
        tv_push_back(vec, tr);
    }

    fclose(fptr); 
}