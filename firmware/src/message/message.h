#ifndef MESSAGES_H
#define MESSAGES_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    #include <stdint.h>
    #include <stdbool.h>

    #define UART_MAX_ATTEMPTS 10000

//    typedef enum : uint8_t {
//        RED,
//        GREEN,
//        BLUE
//    } RGB;
//
//    typedef enum : uint8_t {
//        X,
//        Y,
//        INFO,
//    } FrgType;
//
//    typedef struct {
//        FrgType fragment_type;
//        uint8_t data;
//    } Fragment;

    typedef enum {
        RED,
        GREEN,
        BLUE
    } RGB;

    typedef enum {
        X,
        Y,
        INFO
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

    bool UART_wait(uint32_t module_instance);

    Fragment* new_fragment(FrgType fragment_type, uint8_t data);
    Message* new_message(Fragment x, Fragment y, Fragment info);

    bool send_fragment(Fragment* fragment, RGB rgb);
    bool send_message(Message* msg);
#endif // MESSAGES_H
