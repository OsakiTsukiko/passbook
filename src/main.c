#include "webview.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

#include <vector.h>
#include "passbook_utils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

// Only used to suppress warnings caused by unused parameters.
#define UNUSED(x) (void)x

typedef struct {
  webview_t w;
  vector* transaction_array;
} context_t_gta;

void wv_get_transaction_array(const char *seq, const char *req, void *arg) {
  UNUSED(req);
  context_t_gta *context = (context_t_gta *)arg;
  char* json = transaction_vector_to_json(context->transaction_array);
  printf("%s\n", json);
  webview_return(context->w, seq, 0, json);
}

void wv_add_transaction(const char *seq, const char *req, void *arg) {
  context_t_gta *context = (context_t_gta *)arg;

  transaction* r = transaction_from_json(req);

  tv_push_back(context->transaction_array, r);

  char* json = transaction_vector_to_json(context->transaction_array);
  printf("%s\n", json);
  webview_return(context->w, seq, 0, json);
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
  vector* transaction_array = new_vector();

  tv_push_back(transaction_array, create_transaction("Test Income", 1699377609, TRANSACTION_INCOME, 1500));
  tv_push_back(transaction_array, create_transaction("Test Expense", 1699436141, TRANSACTION_EXPENSE, 750));

  webview_t w = webview_create(1, NULL);
  context_t_gta context = {.w = w, .transaction_array = transaction_array};
  webview_set_title(w, "PassBook");
  webview_set_size(w, 750, 750, WEBVIEW_HINT_NONE);

  // A binding that increments a value and immediately returns the new value.
  webview_bind(w, "get_transaction_array", wv_get_transaction_array, &context);
  webview_bind(w, "wv_add_transaction", wv_add_transaction, &context);

  webview_set_html(w, UI);
  webview_run(w);
  webview_destroy(w);
  
  // char* idk = transaction_vector_to_json(transaction_array);
  // printf("%s\n", idk);
  
  return 0;
}