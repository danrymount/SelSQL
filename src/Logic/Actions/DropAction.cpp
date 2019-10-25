//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DropAction.h"

Error DropAction::execute(std::shared_ptr<BaseActionNode> root) {
    error = getEngine().DropTable(root->getTableName());
    if (error.getErrorCode())
        return Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    return error;
}
