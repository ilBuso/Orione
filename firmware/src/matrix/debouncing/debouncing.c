#include "debouncing.h"

#include <time.h>

uint32_t key_timestamps[NUM_ROWS][NUM_COLS] = {0};

inline uint32_t get_current_timestamp_ms(void){
    return (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
}