//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DropAction.h"
#include "../../Parser/Headers/DropVisitor.h"

Message DropAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<DropVisitor *>(getTreeVisitor().get());
    message = v->getEngine()->DropTable(v->getTableName());
    if (message.getErrorCode())
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    return message;
}
