//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_BASEACTION_H
#define SELSQL_BASEACTION_H

#include "../../../Engine/Headers/MainEngine.h"

class BaseAction {
   protected:
    int errorCode;

   public:
    virtual Response execute(Response& response, MainEngine* mainEngine) = 0;
};

#endif  // SELSQL_BASEACTION_H
