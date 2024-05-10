#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "array.h"
ARRAY_DECL(int)
#define TEST 0
#if TEST
#define BITCOUNT 5
#else
#define BITCOUNT 12
#endif

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x800 ? '1' : '0'), \
  ((byte) & 0x400 ? '1' : '0'), \
  ((byte) & 0x200 ? '1' : '0'), \
  ((byte) & 0x100 ? '1' : '0'), \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

int binary_to_int(const char * line) {
    int result = 0;
    for (size_t i = 0; line[i] && line[i] != '\n'; i++) {
        assert('0' == line[i] || line[i] == '1');
        result *= 2;
        result += line[i] - '0';
    }
    return result;
}

void print_int(int i) {
    printf("%d", i);
}

void print_int_bin(int i) {
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));
}

void filter_oxygen(int bit, Array(int)* numbers, Array(int)* oxygen) {
    int low_bitcount = 0;
    int high_bitcount = 0;
    for (size_t i = 0; i < numbers->len ; i++) {
        //printf("%d len: %zu i: %zu\n", numbers->data[i], numbers->len, i);
        if ((unsigned)numbers->data[i] & 1u << (BITCOUNT -1 - bit)) {
            high_bitcount += 1;
            //printf("set %d \n", numbers->data[i]);
        }
        else {
            low_bitcount += 1;
        }
    }
    printf("bitcounts %d , %d\n", high_bitcount, low_bitcount);
    if (high_bitcount >= low_bitcount) {
        for (size_t i = 0; i < numbers->len; i++) {
            //printf("%d len: %zu i: %zu\n", numbers->data[i], numbers->len, i);
            if ((unsigned)numbers->data[i] & 1u << (BITCOUNT -1 - bit)) {
                //printf("appending high " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(numbers->data[i]));
                arr_int_append(oxygen, numbers->data[i]);
            }
        }
    }
    else if (high_bitcount < low_bitcount) {
        //printf("muhuhuhu\n");
        for (size_t i = 0; i < numbers->len; i++) {
            //printf("%d len: %zu i: %zu\n", numbers->data[i], numbers->len, i);
            if (((unsigned)numbers->data[i] & 1u << (BITCOUNT -1 - bit)) == 0) {
                //printf("appending low " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(numbers->data[i]));
                arr_int_append(oxygen, numbers->data[i]);
            }
        }
    }
}

void filter_co2(int bit, Array(int)* to_filter, Array(int)* co2) {
    int low_bitcount = 0;
    int high_bitcount = 0;
    for (size_t i = 0; i < to_filter->len ; i++) {
        if ((unsigned)to_filter->data[i] & 1u << (BITCOUNT -1 - bit)) {
            high_bitcount += 1;
        }
        else {
            low_bitcount += 1;
        }
    }
    printf("bitcounts %d , %d\n", high_bitcount, low_bitcount);
    if (high_bitcount >= low_bitcount) {
        for (size_t i = 0; i < to_filter->len; i++) {
            if (((unsigned)to_filter->data[i] & 1u << (BITCOUNT -1 - bit)) == 0) {
                arr_int_append(co2, to_filter->data[i]);
            }
        }
    }
    else if (high_bitcount < low_bitcount) {
        for (size_t i = 0; i < to_filter->len; i++) {
            if ((unsigned)to_filter->data[i] & 1u << (BITCOUNT -1 - bit)) {
                arr_int_append(co2, to_filter->data[i]);
            }
        }
    }
}

int main(void) {
    #if TEST
    FILE* file = fopen("test.txt", "r");
    #else
    FILE* file = fopen("input.txt", "r");
    #endif
    assert(file);
    char line[BITCOUNT+10];
    Array(int) numbers = arr_int_init();
    while (fgets(line, BITCOUNT+10, file) != NULL) {
        printf("%s", line);
        arr_int_append(&numbers, binary_to_int(line));
    }
    arr_int_print(&numbers, print_int_bin);
    int bit = 0;
    //Array(int) numbers_copy = arr_int_copy(&numbers);
    Array(int) to_filter = numbers;
    int found = 0;
    while(bit < BITCOUNT) {
        printf("######################\n");
        Array(int) oxygen = arr_int_init();
        printf("bit %d\ntofilter: ", bit);
        arr_int_print(&to_filter, print_int_bin);
        filter_oxygen(bit, &to_filter, &oxygen);
        printf("oxygen: ");
        arr_int_print(&oxygen, print_int_bin);
        if (oxygen.len == 1) {
            found = oxygen.data[0];
            //arr_int_deinit(oxygen);
            goto found;
        }
        to_filter = oxygen;
        bit += 1;
    }
    assert(0 && "Not found");
    found:
        printf("found: %d\n\n\n", found);
    to_filter = numbers;
    int found2 = 0;
    bit = 0;
    while (bit < BITCOUNT) {
        printf("######################\n");
        Array(int) co2 = arr_int_init();
        printf("bit: %d, to_filter: ", bit);
        arr_int_print(&to_filter, print_int_bin);
        filter_co2(bit, &to_filter, &co2);
        printf("co2: ");
        arr_int_print(&co2, print_int_bin);
        if (co2.len == 1) {
            found2 = co2.data[0];
            goto found2;
        }
        to_filter = co2;
        bit += 1;
    }
    assert(0 && "Not found");
    found2:
        printf("found %d\n\n\n", found2);
    printf("%d", found * found2);
    return 0;
}
