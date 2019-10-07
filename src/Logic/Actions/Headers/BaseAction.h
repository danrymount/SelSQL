//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_BASEACTION_H
#define SELSQL_BASEACTION_H

#include "../../../Engine/Headers/MainEngine.h"
#include "../../../Utils/Structures/BigResponse.h"

class BaseAction {
   protected:
    int errorCode;

   public:
    virtual BigResponse execute(BigRequest& _request, MainEngine* mainEngine) = 0;

    BigResponse response;
};

#endif  // SELSQL_BASEACTION_H
