#ifndef MESSAGES_H
#define MESSAGES_H

    #include <stdint.h>
    #include <stdbool.h>

    typedef enum {
        INIT = 0,
        X_MSG_TYPE,
        X_DATA,
        Y_MSG_TYPE,
        Y_DATA,
        INFO_MSG_TYPE,
        INFO_DATA,
    } Communication;
    
    typedef enum {
        INFO,
        X,
        Y
    } MsgType;

    typedef struct {
        MsgType msg_type;
        uint8_t data;
    } Message;

    Message receive_message();
    void assemble_message();
    bool check_order();

#endif // MESSAGES_H