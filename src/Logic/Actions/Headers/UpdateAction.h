//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_UPDATEACTION_H
#define SELSQL_UPDATEACTION_H

#include "BaseAction.h"
class UpdateAction : public BaseAction {
   public:
    explicit UpdateAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Error execute(std::shared_ptr<BaseActionNode>) override;

   private:
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    //BigResponse execute(std::shared_ptr<BigRequest>, MainEngine* mainEngine) override;
};

#endif  // SELSQL_UPDATEACTION_H
