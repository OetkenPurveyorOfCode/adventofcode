#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define TEST 1
int main(void) {
    #if TEST
    FILE* file = fopen("test.txt", "r");
    #define BITCOUNT 5
    #else
    FILE* file = fopen("input.txt", "r");
    #define BITCOUNT 12
    #endif
    assert(file);
    char line[BITCOUNT +10];
    int bitcounts[BITCOUNT] = {0};
    int total = 0;
    while (fgets(line, BITCOUNT +10, file) != NULL) {
        printf("%s", line);
        for (int i = 0; i < BITCOUNT; i++) {
            if (line[i] == '1') {
                bitcounts[i] += 1;
            }
            else if (line[i] == '0') {
            } else {

                assert(0);
            }
        }
        total += 1;
    }
    unsigned gamma = 0;
    for (unsigned int i = 0; i < BITCOUNT; i++) {
        if (2*bitcounts[i] > total) {
            gamma |= 1u << (BITCOUNT - 1  - i);
        } 
    }
    #if TEST
    unsigned epsilon = (~gamma) & 0b00011111u;
    #else
    unsigned epsilon = (~gamma) & 0b111111111111u;
    #endif
    printf("%d %x %d\n", gamma, epsilon, gamma*epsilon);
    return 0;
}
