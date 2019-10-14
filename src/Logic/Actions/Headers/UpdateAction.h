//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_UPDATEACTION_H
#define SELSQL_UPDATEACTION_H

#include "BaseAction.h"
class UpdateAction : public BaseAction {
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;

   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;
};

#endif  // SELSQL_UPDATEACTION_H
