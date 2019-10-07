//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONSELECT_H
#define SELSQL_ACTIONSELECT_H

#include "BaseAction.h"
class ActionSelect : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONSELECT_H
