//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_DELETEACTION_H
#define SELSQL_DELETEACTION_H

#include "BaseAction.h"
class DeleteAction : public BaseAction {
   public:
    explicit DeleteAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Error execute(std::shared_ptr<BaseActionNode>) override;

   private:
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
};

#endif  // SELSQL_DELETEACTION_H
