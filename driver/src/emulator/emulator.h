#ifndef EMULATOR_H
#define EMULATOR_H

    #include "../message/message.h"

    void emulator_linux(Message* msg, int uinput_fd);
    void emulator_macos();
    void emulator_win64();

#endif // EMULATOR_H