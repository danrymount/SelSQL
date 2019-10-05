//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONINSERT_H
#define SELSQL_ACTIONINSERT_H

#include "BaseAction.h"
class ActionInsert : public BaseAction {
   public:
    Response execute(Response& response, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONINSERT_H
