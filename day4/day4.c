#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "array.h"
#define MARKED 0x80000000
typedef struct {
    uint32_t nums[5][5];
} Board;

ARRAY_DECL(int)
ARRAY_DECL(Board)

typedef enum {
    ParsingDelimiter,
    ParsingNumber,
} State;

void print_int(int i) {
    printf("%d", i);
}

void print_Board(Board board) {
    printf("\n[");
    for (int i = 0; i < 5; i++) {
        printf("[");
        for (int j = 0; j < 5; j++) {
            if (board.nums[i][j] & MARKED) {
                printf("*%2d, ", board.nums[i][j] & ~MARKED);
            }
            else {
                printf(" %2d, ", board.nums[i][j]);
            }
        }
        printf("]\n");
    }
    printf("]");
}

void parse_board(Array(Board)* boards, FILE *file) {
    State state = ParsingDelimiter;
    uint32_t current_number = 0;
    size_t current_row_index = 0;
    Board board = {{0}};
    char buffer[32*1024] = {0};
    for (int j = 0; j < 5; j++) {
        memset(buffer, 0, sizeof(buffer));
        assert(fgets(buffer, sizeof(buffer), file) != NULL);
        size_t current_col_index = 0;
        for (int i = 0;; i++) {
            switch(state) {
                break;case ParsingDelimiter: {
                    if ('0' <= buffer[i] && buffer[i] <= '9') {
                        state = ParsingNumber;
                        current_number = (uint32_t)buffer[i] - '0';
                    }
                }
                break;case ParsingNumber: {
                    if ('0' <= buffer[i] && buffer[i] <='9') {
                        current_number *= 10;
                        current_number += (uint32_t)buffer[i] - '0';
                    }
                    else {
                        state = ParsingDelimiter;
                        assert(current_col_index < 5);
                        board.nums[current_row_index][current_col_index] = current_number;
                        current_col_index += 1;
                    }
                }
            }
            if (buffer[i] == '\n') {
                break;
            }
        }  
        current_row_index += 1;
    }
    arr_Board_append(boards, board);
}
#define nonconst
bool handle_number(uint32_t number, nonconst Board* board) {
    bool winning_cols[5] = {true, true, true, true, true};
    bool winning_rows[5] = {true, true, true, true, true};
    for (int row = 0; row < 5; row++) {
        for(int col = 0; col < 5; col++) {
            if (board->nums[row][col] == number) {
                board->nums[row][col] |= MARKED;
            }
            if (!(board->nums[row][col] & MARKED)) {
                winning_rows[row] = false;
                winning_cols[col] = false;
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        if (winning_cols[i] || winning_rows[i]) {
            return true;
        }
    }
    return false;
}

int calculate_score(Board board, int number) {
    int sum_of_unmarked_numbers = 0;
    for(int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (!(board.nums[row][col] & MARKED)) {
                sum_of_unmarked_numbers += board.nums[row][col];
            }
        }
    }
    return sum_of_unmarked_numbers * number;

}

int main(void) {
    FILE* file = fopen("input.txt", "r");
    assert(file);
    char buffer[32*1024] = {0};
    fgets(buffer, sizeof(buffer), file);
    printf("first line %s EOL\n", buffer);
    State state = ParsingDelimiter;
    int current_number = 0;
    Array(int) numbers = arr_int_init();
    for (int i = 0; buffer[i] != '\n'; i++) {
        switch(state) {
            break;case ParsingDelimiter: {
                if ('0' <= buffer[i] && buffer[i] <= '9') {
                    state = ParsingNumber;
                    current_number = buffer[i] - '0';
                }
            }
            break;case ParsingNumber: {
                if ('0' <= buffer[i] && buffer[i] <= '9') {
                    current_number *= 10;
                    current_number += buffer[i] -'0';
                }
                else {
                    state = ParsingDelimiter;
                    arr_int_append(&numbers, current_number);
                }
            }
        }
    }
    if (state == ParsingNumber) {
        arr_int_append(&numbers, current_number);
    }
    arr_int_print(&numbers, print_int);

    Array(Board) boards = arr_Board_init();
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        parse_board(&boards, file);
    }
    arr_Board_print(&boards, print_Board);

    Board winning_board;
    int winning_number;
    for (int num_it = 0; num_it < numbers.len; num_it++) {
        for (int board_it = 0; board_it < boards.len; board_it++) {
            if (handle_number(numbers.data[num_it], &boards.data[board_it])){
                winning_board = boards.data[board_it];
                winning_number = numbers.data[num_it];
                goto score;
            }
        }
    }

score:
    printf("Winning Board: \n");
    print_Board(winning_board);
    int final_score = calculate_score(winning_board, winning_number);
    printf("%d", final_score);
    /*
    printf("\n\n");
    arr_Board_print(&boards, print_Board);*/
defer:
    arr_int_deinit(numbers);
    arr_Board_deinit(boards);
    return 0;
}
