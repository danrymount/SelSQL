//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONSHOWCREATE_H
#define SELSQL_ACTIONSHOWCREATE_H

#include "BaseAction.h"
class ActionShowCreate : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONSHOWCREATE_H
