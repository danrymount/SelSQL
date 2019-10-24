//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_CREATEACTION_H
#define SELSQL_CREATEACTION_H

#include "BaseAction.h"
class CreateAction : public BaseAction {
   public:
    explicit CreateAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Error execute(std::shared_ptr<BaseActionNode>) override;
};

#endif  // SELSQL_CREATEACTION_H
