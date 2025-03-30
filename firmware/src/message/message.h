#ifndef MESSAGES_H
#define MESSAGES_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    #include <stdint.h>
    #include <stdbool.h>

    typedef enum : uint8_t {
        RED,
        GREEN,
        BLUE
    } RGB;

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

    // MSP432
    Fragment* new_fragment(FrgType fragment_type, uint8_t data);
    Message* new_message(Fragment x, Fragment y, Fragment info);

    bool send_fragment(Fragment* fragment, RGB rgb);
    bool send_message(Message* msg);
#endif // MESSAGES_H