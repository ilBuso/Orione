#ifndef MESSAGES_H
#define MESSAGES_H

    #include <stdint.h>
    #include <stdbool.h>

    typedef enum {
        RED,
        GREEN,
        BLUE
    } RGB;

    typedef enum {
        INFO,
        X,
        Y
    } MsgType;

    typedef struct {
        MsgType msg_type;
        uint8_t data;
    } Message;

    // MSP432
    Message* new_message(MsgType msg_type, uint8_t data);
    void fragment_message(Message x, Message y, Message Info);
    bool send_message(Message msg, RGB rgb);
#endif // MESSAGES_H