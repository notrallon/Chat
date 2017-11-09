#include "comfuncs.h"


void ComFuncs::SetName(std::string name, SocketThread* thread)
{
    thread->SetName(name);
}
