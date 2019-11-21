//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ShowCreateAction.h"
#include "../../Parser/Headers/ShowCreateVisitor.h"
#include "../../Utils/Headers/ActionsUtils.h"

Message ShowCreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<ShowCreateVisitor *>(getTreeVisitor().get());
    auto table = v->getEngine()->ShowCreateTable(v->getTableName());
    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    return Message(ActionsUtils::makeRequestCreateFromTable(table));
}
