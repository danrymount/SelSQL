//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONDELETE_H
#define SELSQL_ACTIONDELETE_H

#include "BaseAction.h"
class ActionDelete : public BaseAction {
   public:
    Response execute(Response& response, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONDELETE_H
