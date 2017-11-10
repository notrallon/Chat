#include "comfuncs.h"


void ComFuncs::SetName(std::string name, SocketThread* thread)
{
    thread->SetName(name);
}

void ComFuncs::ConfirmClient(std::string buffer, SocketThread *thread)
{
    thread->SendConfirmation(buffer);
}
