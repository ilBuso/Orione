#ifndef EMULATOR_H
#define EMULATOR_H

    #include "../message/message.h"
    #include "../global/global.h"

    bool emulate_key(Message* msg, int uinput_fd);

#endif // EMULATOR_H