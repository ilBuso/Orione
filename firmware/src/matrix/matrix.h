#ifndef MATRIX_H
#define MATRIX_H

    #define MAX_KEYS 6
    typedef struct {
        volatile bool has_new_key;
        volatile uint8_t pressed_keys_count;
        volatile uint8_t pressed_keys[MAX_KEYS][2]; // [row, column] pairs
        volatile uint8_t current_layer;
    } keyboard_state_t;

    #define ROW_0 0
    #define ROW_1 1
    #define ROW_2 2
    #define ROW_3 3
    #define ROW_4 4

    #define COLUMN_0 5
    #define COLUMN_1 6
    #define COLUMN_2 7
    #define COLUMN_3 8
    #define COLUMN_4 9
    #define COLUMN_5 10
    #define COLUMN_6 11
    #define COLUMN_7 12
    #define COLUMN_8 13
    #define COLUMN_9 14
    #define COLUMN_10 15
    #define COLUMN_11 16
    #define COLUMN_12 17
    #define COLUMN_13 18

    #define FN_KEY_ROW 4
    #define FN_KEY_COL 9

#endif /* MATRIX_H */