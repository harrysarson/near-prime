#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


char get_tweaked_char(char c) {
	switch(c) {
		case '0' : return '8'; break;
		case '1' : return '7'; break;
		case '2' : return '5'; break;
		case '3' : return '4'; break;
		case '4' : return '3'; break;
		case '5' : return '2'; break;
		case '6' : return '9'; break;
		case '7' : return '1'; break;
		case '8' : return '0'; break;
		case '9' : return '6'; break;
		default : return c;
	}
}

void tweak_string(char * s, int i) {
	s[i] = get_tweaked_char(s[i]);
}

int miller_rabin(const char * img, int reps) {
	mpz_t n;
	mpz_init(n);
	mpz_set_str(n, img, 10); // read from string as base 10
	int ret = mpz_probab_prime_p(n, reps);
	mpz_clear(n);
	return ret;
}

int bitset_get(uint8_t * bitset, int i) {
	return bitset[i / 8] & (1UL << (i % 8));
}

void bitset_set(uint8_t * bitset, int i, int new_value) {
  unsigned long newbit = !!new_value;
  bitset[i / 8] ^= (-newbit ^ bitset[i / 8]) & (1UL << (i % 8));
}


int prev_perm(uint8_t * bitset, int N) {
  int pivot = N - 1;

  for (; pivot >= 0 && bitset_get(bitset, pivot); pivot--);

  for (; pivot >= 0 && !bitset_get(bitset, pivot); pivot--);

  if (pivot < 0) {
    return 0;
  }

  int rightMostFalseIndex = N - 1;

  for (; bitset_get(bitset, rightMostFalseIndex); rightMostFalseIndex--);

  bitset_set(bitset, pivot, 0);
  bitset_set(bitset, rightMostFalseIndex, 1);

  // Reverse everything after the pivot

  int k = N;

  pivot++;
  k--;

  while (pivot < k) {
    int tmp = bitset_get(bitset, k);
    bitset_set(bitset, k, bitset_get(bitset, pivot));
    bitset_set(bitset, pivot, tmp);
    pivot++;
    k--;
  }

  return 1;
}


int comb(const char * src, char * dest, int K) {
	int N = strlen(src);
	int i;

	uint8_t * const bitmask = calloc((N - 1)/ 8 + 1, sizeof(*bitmask));
	for (i = 0; i < K; i++) {
		bitset_set(bitmask, i, 1);
	}

    // print integers and permute bitmask
	int res = 0;
	do {
		strcpy(dest, src); 
		dest[N-1] = '1';
	    for (i = 0; i < N; ++i) {
	        if (bitset_get(bitmask, i)) tweak_string(dest, i);
	    }
		res = miller_rabin(dest, 8);
		if (res != 0) {
			break;
		}
	} while (prev_perm(bitmask, N));

 	free(bitmask);
	return res;
}

int find_candidate(char * str) {
	const int len = strlen(str);
	char * const buffer = malloc(len + 1);
	int i;
	int res = 0;
	for (i = 0; i < len; i++) {
		res = comb(str, buffer, i);
		if (res != 0) {
			strcpy(str, buffer);
			break;
		}
	}
	free(buffer);
	return res;
}
