//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_CREATEACTION_H
#define SELSQL_CREATEACTION_H

#include "BaseAction.h"
class CreateAction : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_CREATEACTION_H