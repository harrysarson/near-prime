#include <node_api.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "prime_search.h"

napi_value wrapped_find_candidate(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  if (argc < 1) {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  size_t string_length;
  status = napi_get_value_string_utf8(env, args[0], NULL, 0, &string_length);
  string_length++; // for \0
  
  if (status == napi_string_expected) {
    napi_throw_type_error(env, NULL, "String expected as second argument");
    return NULL;
  }
  
  assert(status == napi_ok);

  char * const string = malloc(string_length);
  status = napi_get_value_string_utf8(env, args[0], string, string_length, NULL);
  assert(status == napi_ok);

  const int32_t result = find_candidate(string);
  
  napi_value js_result;
  status = napi_create_int32(env, result, &js_result);
  assert(status == napi_ok);

  napi_value prime_string;
  status = napi_create_string_utf8(env, string, string_length, &prime_string);
  assert(status == napi_ok);

  napi_value ret;
  status = napi_create_array_with_length(env, 2, &ret);
  assert(status == napi_ok);

  status = napi_set_element(env, ret, 0, js_result);
  assert(status == napi_ok);

  status = napi_set_element(env, ret, 1, prime_string);
  assert(status == napi_ok);

  free(string);

  return ret;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("prime_search", wrapped_find_candidate);
  status = napi_define_properties(env, exports, 1, &addDescriptor);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(addon, Init)