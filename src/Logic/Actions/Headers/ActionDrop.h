//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONDROP_H
#define SELSQL_ACTIONDROP_H

#include "BaseAction.h"
class ActionDrop : public BaseAction {
   public:
    Response execute(Response& response, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONDROP_H
