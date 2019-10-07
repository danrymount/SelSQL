//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_DELETEACTION_H
#define SELSQL_DELETEACTION_H

#include "BaseAction.h"
class DeleteAction : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_DELETEACTION_H
