//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_BASEACTION_H
#define SELSQL_BASEACTION_H

#include <utility>

#include "../../../Engine/Headers/MainEngine.h"
#include "../../../Parser/Nodes/ActionNodes/BaseActionNode.h"
#include "../../../Parser/Nodes/BaseNode.h"
#include "../../../Utils/Headers/ActionsUtils.h"

class BaseAction {
   public:
    explicit BaseAction(std::shared_ptr<TreeVisitor> _visitor) : visitor(std::move(_visitor)) {}

    virtual Message execute(std::shared_ptr<BaseActionNode>) = 0;

    std::shared_ptr<TreeVisitor> getTreeVisitor() { return visitor; }

   protected:
    ActionsUtils actionsUtils;
    Message message;

   private:
    // shared ptr
    std::shared_ptr<TreeVisitor> visitor;
};

#endif  // SELSQL_BASEACTION_H
