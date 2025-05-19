#ifndef MESSAGES_H
#define MESSAGES_H

    #include <stdint.h>
    #include <stdbool.h>

    #define MAX_WAIT_CYCLES 50000

    typedef enum : uint8_t {
        INIT = 0,
        X_TYPE,
        X_DATA,
        Y_TYPE,
        Y_DATA,
        INFO_TYPE,
        INFO_DATA
    } Communication;
    
    typedef enum : uint8_t {
        X,
        Y,
        INFO,
    } FrgType;

    typedef struct {
        FrgType fragment_type;
        uint8_t data;
    } Fragment;

    typedef struct {
        Fragment x;
        Fragment y;
        Fragment info;
    } Message;

    Fragment* receive_fragment(Communication current_step);
    Message* receive_message();
#endif // MESSAGES_H