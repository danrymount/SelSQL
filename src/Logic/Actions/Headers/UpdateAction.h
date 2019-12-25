//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_UPDATEACTION_H
#define SELSQL_UPDATEACTION_H

#include "../../../Parser/Headers/ExpressionOptimizerVisitor.h"
#include "../../../Parser/Headers/ExpressionVisitor.h"
#include "BaseAction.h"
class UpdateAction : public BaseAction {
   public:
    explicit UpdateAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {
        exprVisitor = new ExpressionVisitor();
        optimizerExprVisitor = new ExpressionOptimizerVisitor();
    }

    ~UpdateAction() { delete exprVisitor; }

    Message execute(std::shared_ptr<BaseActionNode>) override;

   private:
    ExpressionOptimizerVisitor* optimizerExprVisitor;
    ExpressionVisitor* exprVisitor;
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
};

#endif  // SELSQL_UPDATEACTION_H
