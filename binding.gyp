{
  "targets": [
    {
      "target_name": "native_prime_search",
      'link_settings': {
        'libraries': ['-lgmp'],
      },
      "sources": [ "cc-src/addon.c", "cc-src/prime_search.c" ]
    }
  ]
}
