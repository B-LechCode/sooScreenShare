#ifndef TRANSPORTSERVERTCP_H
#define TRANSPORTSERVERTCP_H

#include "itransportserver.h"

namespace trans {
    #define PORT
}

class transportServerTCP : public ItransportServer
{
public:
    transportServerTCP();
    virtual ~transportServerTCP();
};

#endif // TRANSPORTSERVERTCP_H
