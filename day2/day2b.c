#include <stdio.h>
#include <assert.h>

int main(void) {
    FILE* file = fopen("input.txt", "r");
    assert(file);
    int x_pos = 0;
    int depth = 0;
    int aim = 0;
    int delta;
    while (!feof(file)) {
        if(fscanf(file, "forward %d\n", &delta) != 0) {
            printf("forward %d\n", delta);
            x_pos += delta;
            depth += aim * delta;
        }
        if(fscanf(file, "down %d\n", &delta) != 0) {
            printf("down %d\n", delta);
            aim += delta;
        }
        if(fscanf(file, "up %d\n", &delta) != 0) {
            printf("up %d\n", delta);
            aim -= delta;
        }
    }
    printf("%d, %d, %d", x_pos, depth, x_pos * depth);
    fclose(file);
    return 0;
}
