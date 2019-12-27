//
// Created by sapiest on 24.12.2019.
//

#ifndef SELSQL_INDEXCREATEACTION_H
#define SELSQL_INDEXCREATEACTION_H

#include "BaseAction.h"
class IndexCreateAction : public BaseAction {
   public:
    explicit IndexCreateAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Message execute(std::shared_ptr<BaseActionNode> root) override;
};

#endif  // SELSQL_INDEXCREATEACTION_H
