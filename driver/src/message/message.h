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
        INFO_DATA
    } Communication;
    
    typedef enum {
        X,
        Y,
        INFO,
        END
    } MsgType;

    typedef struct {
        uint8_t x;
        uint8_t y;
        uint8_t info;
    } Message;

    Message receive_message_linux(int serial_fd);
    Message receive_message_macos();
    Message receive_message_win64();
    void emulate_key_linux(Message msg, int uinput_fd);
    void emulate_key_macos();
    void emulate_key_win64();

#endif // MESSAGES_H