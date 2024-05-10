#include <stdio.h>
#include <limits.h>

int main() {
    FILE* f = fopen("input.txt", "r");
    int number = 0;
    int previous_number = INT_MIN;
    int increased = 0;
    for(;;) {
        int c = fgetc(f);
        if ('0' <= c && c <= '9') {
            number = number*10+(c-'0');
        }
        else {
            printf("%d %d\n", previous_number, number);
            if (previous_number < number && previous_number != INT_MIN) {
                printf("increased\n");
                increased++;
            }
            previous_number = number;
            number = 0;
        }
        if (c == EOF) {
            break;
        }
    };
    fclose(f);
    printf("increased %d times\n", increased);
    fprintf(stderr, "%d", increased);
}