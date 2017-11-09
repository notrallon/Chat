#ifndef COMFUNCS_H
#define COMFUNCS_H

#include "socketthread.h"

struct ComFuncs
{
    static void SetName(std::string name, SocketThread* thread);
};

#endif // COMFUNCS_H
