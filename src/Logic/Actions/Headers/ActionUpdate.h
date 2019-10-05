//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONUPDATE_H
#define SELSQL_ACTIONUPDATE_H

#include "BaseAction.h"
class ActionUpdate : public BaseAction {
   public:
    Response execute(Response& response, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONUPDATE_H
