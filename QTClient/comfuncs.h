#ifndef COMFUNCS_H
#define COMFUNCS_H

#include "socketthread.h"

struct ComFuncs
{
    static void SetName(std::string name, SocketThread* thread);
    static void ConfirmClient(std::string buffer, SocketThread* thread);
};

#endif // COMFUNCS_H
