//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_INSERTACTION_H
#define SELSQL_INSERTACTION_H

#include "BaseAction.h"
class InsertAction : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_INSERTACTION_H
