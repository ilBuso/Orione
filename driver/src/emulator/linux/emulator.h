#ifndef EMULATOR_H
#define EMULATOR_H

    #include "../../message/message.h"
    #include "../../global/global.h"
    #include "../../parser/parser.h"

    bool emulate_key(Message* msg, int uinput_fd, const Parse* parse);

#endif // EMULATOR_H