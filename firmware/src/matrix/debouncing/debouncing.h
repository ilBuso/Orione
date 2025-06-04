#ifndef DEBOUNCING_H
#define DEBOUNCING_H

    #include <stdint.h>
    #include "../global/global.h"

    extern uint32_t key_timestamps[NUM_ROWS][NUM_COLS];
    inline uint32_t get_current_timestamp_ms(void);

#endif
