//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_DROPACTION_H
#define SELSQL_DROPACTION_H

#include "BaseAction.h"
class DropAction : public BaseAction {
   public:
    BigResponse execute(std::shared_ptr<BigRequest>, MainEngine* mainEngine) override;
};

#endif  // SELSQL_DROPACTION_H
