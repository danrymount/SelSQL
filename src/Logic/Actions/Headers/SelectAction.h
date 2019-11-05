//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SELECTACTION_H
#define SELSQL_SELECTACTION_H

#include <sstream>
#include "../../../Parser/Headers/ExpressionVisitor.h"
#include "BaseAction.h"
class SelectAction : public BaseAction {
   public:
    std::stringstream stringstream;

    explicit SelectAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {
        exprVisitor = new ExpressionVisitor();
    }

    ~SelectAction() { delete exprVisitor; }

    Message execute(std::shared_ptr<BaseActionNode>) override;

   private:
    ExpressionVisitor *exprVisitor;
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> records;
};

#endif  // SELSQL_SELECTACTION_H
