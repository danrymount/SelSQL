//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DropAction.h"

Message DropAction::execute(std::shared_ptr<BaseActionNode> root) {
    message = getEngine().DropTable(root->getTableName());
    if (message.getErrorCode())
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    return message;
}
