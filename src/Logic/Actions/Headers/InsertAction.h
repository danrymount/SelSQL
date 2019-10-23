//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_INSERTACTION_H
#define SELSQL_INSERTACTION_H

#include "../../../Parser/Nodes/ActionNodes/BaseActionNode.h"
#include "../../../Parser/Nodes/BaseNode.h"
#include "BaseAction.h"
class InsertAction : public BaseAction {
   public:
    explicit InsertAction(std::shared_ptr<TreeVisitor> _visitor)
                                                                                                        : BaseAction(std::move(_visitor)) {
    }

    Error execute(std::shared_ptr<BaseActionNode>) override;

   private:
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    // std::vector<std::pair<std::string, std::string>> record;
};

#endif  // SELSQL_INSERTACTION_H
