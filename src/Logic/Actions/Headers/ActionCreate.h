//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONCREATE_H
#define SELSQL_ACTIONCREATE_H

#include "BaseAction.h"
class ActionCreate : public BaseAction {
   public:
    Response execute(Response& response, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONCREATE_H
