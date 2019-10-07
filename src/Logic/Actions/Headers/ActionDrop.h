//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONDROP_H
#define SELSQL_ACTIONDROP_H

#include "BaseAction.h"
class ActionDrop : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_ACTIONDROP_H
