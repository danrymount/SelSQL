//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_INSERTACTION_H
#define SELSQL_INSERTACTION_H

#include "BaseAction.h"
class InsertAction : public BaseAction {
   public:
    BigResponse execute(std::shared_ptr<BigRequest>, MainEngine* mainEngine) override;

   private:
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    // std::vector<std::pair<std::string, std::string>> record;
};

#endif  // SELSQL_INSERTACTION_H
