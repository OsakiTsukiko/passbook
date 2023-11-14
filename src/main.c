#include "webview.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

#include <vector.h> // part of my library (mystd) found on my github at https://github.com/OsakiTsukiko/mystd
#include "passbook_utils.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <pthread.h>
#include <unistd.h>
#define F_OK 0
#endif

// Only used to suppress warnings caused by unused parameters.
#define UNUSED(x) (void)x

// struct used to give code context to ui binded apps without global varibles
// a functionality of the ui library
typedef struct {
  webview_t w;
  vector* transaction_array;
} context_t_gta;

// function binded to the ui
// returns a json with all elements of the transaction vector (to the ui via webview_return)
void wv_get_transaction_array(const char *seq, const char *req, void *arg) {
  UNUSED(req);
  context_t_gta *context = (context_t_gta *)arg;
  if (context->transaction_array->len > 0) {
    char* json = transaction_vector_to_json(context->transaction_array);
    printf("%s\n", json);
    webview_return(context->w, seq, 0, json);
  } else {
    webview_return(context->w, seq, 0, "[]");
  }
}

// function binded to the ui
// adds a transaction to the transaction vector
// returns a json with all elements of the transaction vector (to the ui via webview_return)
void wv_add_transaction(const char *seq, const char *req, void *arg) {
  context_t_gta *context = (context_t_gta *)arg;

  transaction* r = transaction_from_json(req);

  tv_push_back(context->transaction_array, r);

  if (context->transaction_array->len > 0) {
    char* json = transaction_vector_to_json(context->transaction_array);
    printf("%s\n", json);
    webview_return(context->w, seq, 0, json);
  } else {
    webview_return(context->w, seq, 0, "[]");
  }
}

// function binded to the ui
// removes a transaction from the transaction vector
// returns a json with all elements of the transaction vector (to the ui via webview_return)
void wv_get_transactions_between_dates(const char *seq, const char *req, void *arg) {
  context_t_gta *context = (context_t_gta *)arg;

  vector* r = transaction_vec_from_dates(req, context->transaction_array);

  if (r->len > 0) {
    char* json = transaction_vector_to_json(r);
    printf("%s\n", json);
    webview_return(context->w, seq, 0, json);
  } else {
    webview_return(context->w, seq, 0, "[]");
  }
}

// function binded to the ui
// returns a json with all elements of the transaction vector between the two specified dates (to the ui via webview_return)
void wv_remove_transaction(const char *seq, const char *req, void *arg) {
  context_t_gta *context = (context_t_gta *)arg;

  int r = transaction_id_from_json(req);
  if (r < context->transaction_array->len) {
    transaction* t = tv_remove(context->transaction_array, r);
    destroy_transaction(t);
  }

  if (context->transaction_array->len > 0) {
    char* json = transaction_vector_to_json(context->transaction_array);
    printf("%s\n", json);
    webview_return(context->w, seq, 0, json);
  } else {
    webview_return(context->w, seq, 0, "[]");
  }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow) {
  (void)hInst;
  (void)hPrevInst;
  (void)lpCmdLine;
  (void)nCmdShow;
#else
int main() {
#endif
  // new vector of void pointers 
  vector* transaction_array = new_vector();

  if (access("storage.txt", F_OK) == 0) {
    load_transaction_vec("storage.txt", transaction_array);
  } else {
    tv_push_back(transaction_array, create_transaction("Test Income", 1699377609, TRANSACTION_INCOME, 1500));
    tv_push_back(transaction_array, create_transaction("Test Expense", 1699436141, TRANSACTION_EXPENSE, 750));
  }

  // push 2 test transactions to the vector
  // tv_push_back(transaction_array, create_transaction("Test Income", 1699377609, TRANSACTION_INCOME, 1500));
  // tv_push_back(transaction_array, create_transaction("Test Expense", 1699436141, TRANSACTION_EXPENSE, 750));

  // initialize ui
  webview_t w = webview_create(1, NULL);
  context_t_gta context = {.w = w, .transaction_array = transaction_array}; // initialize context
  webview_set_title(w, "PassBook");
  webview_set_size(w, 750, 750, WEBVIEW_HINT_NONE);

  // add bindings
  webview_bind(w, "get_transaction_array", wv_get_transaction_array, &context);
  webview_bind(w, "wv_add_transaction", wv_add_transaction, &context);
  webview_bind(w, "remove_transaction", wv_remove_transaction, &context);
  webview_bind(w, "get_transactions_between_dates", wv_get_transactions_between_dates, &context);

  webview_set_html(w, UI);
  webview_run(w);
  webview_destroy(w);

  dump_transaction_vec("storage.txt", transaction_array);
  
  return 0;
}