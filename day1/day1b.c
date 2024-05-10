#include <limits.h>
#include <stdio.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    int numbers[3] = {0, INT_MIN, INT_MIN};
    int sum = INT_MIN;
    int previous_sum = INT_MIN;
    int increased = 0;
    for (;;) {
        int c = fgetc(f);
        if ('0' <= c && c <= '9') {
            numbers[0] = numbers[0] * 10 + (c - '0');
        } else {
            if (numbers[1] != INT_MIN && numbers[2] != INT_MIN) {
                sum = numbers[0] + numbers[1] + numbers[2];
            }
            if (previous_sum < sum && previous_sum != INT_MIN) {
                increased++;
                printf("increased\n");
            }
            if (sum != INT_MIN) {
                previous_sum = sum;
            }
            numbers[2] = numbers[1];
            numbers[1] = numbers[0];
            numbers[0] = 0;
        }
        if (c == EOF) {
            break;
        }
    };
    fclose(f);
    printf("increased %d times\n", increased);
    fprintf(stderr, "%d", increased);
}