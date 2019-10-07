//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SELECTACTION_H
#define SELSQL_SELECTACTION_H

#include "BaseAction.h"
class SelectAction : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_SELECTACTION_H
