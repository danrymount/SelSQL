//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ShowCreateAction.h"
#include "../../Utils/Headers/ActionsUtils.h"

BigResponse ShowCreateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    ActionsUtils actionsUtils;
    response.ddlData.table = *mainEngine->ShowCreateTable(_request.tableName);
    // response.table =
    if (&(response.ddlData.table) == nullptr) {
        // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }
    response.ddlData.returnMsg = actionsUtils.makeRequestCreateFromTable(response.ddlData.table);
    // std::cout << response.returnMsg  << std::endl;
    return response;
}
