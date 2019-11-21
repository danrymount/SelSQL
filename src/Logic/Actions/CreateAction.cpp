//
// Created by sapiest on 05.10.2019.
//

#include "Headers/CreateAction.h"
#include "../../Parser/Headers/CreateVisitor.h"
Message CreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<CreateVisitor*>(getTreeVisitor().get());
    auto t = v->getTable();
    auto error = v->getError();
    if (error.getErrorCode()) {
        return error;
    }
    return v->getEngine()->CreateTable(std::make_shared<Table>(v->getTable()));
}
