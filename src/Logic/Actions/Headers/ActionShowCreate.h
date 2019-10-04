//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONSHOWCREATE_H
#define SELSQL_ACTIONSHOWCREATE_H

#include "BaseAction.h"
class ActionShowCreate: public BaseAction{
   public:
    Response execute(Response &response) override;
};

#endif  // SELSQL_ACTIONSHOWCREATE_H
