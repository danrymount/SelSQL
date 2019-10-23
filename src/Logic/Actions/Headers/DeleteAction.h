//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_DELETEACTION_H
#define SELSQL_DELETEACTION_H

#include "BaseAction.h"
class DeleteAction : public BaseAction {
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;

   public:
    //BigResponse execute(std::shared_ptr<BigRequest>, MainEngine* mainEngine) override;
};

#endif  // SELSQL_DELETEACTION_H
