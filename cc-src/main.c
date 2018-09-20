#include "prime_search.h"
#include <stdio.h>

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Please provide a number to turn into a prime\n");
        return 0;
    } else {
        char * number = argv[1];
        int res = find_candidate(number);

        switch (res) {
            case 0:
                printf("Cannot find a prime number\n");
                break;
            case 1: 
                printf("Found a probably prime number:\n%s\n", number);
                break;
            case 2:
                printf("Found a definately prime number:\n%s\n", number);
                break;
            default:
                printf("Error: got return code = %d\n", res);
        }

        return (res == 1) || (res == 2);
    }
}