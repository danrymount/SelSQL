//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionShowCreate.h"
#include "../../Utils/Headers/ActionsUtils.h"

BigResponse ActionShowCreate::execute(BigRequest& _request, MainEngine* mainEngine) {
    ActionsUtils actionsUtils;
    response.table = *mainEngine->ShowCreateTable(_request.tableName);
    // response.table =
    if (&(response.table) == nullptr) {
        // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
        response.error.errorCode = 1;
        response.error.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
        return response;
    }
    response.ddlData.returnMsg = actionsUtils.makeRequestCreateFromTable(response.ddlData.table);
    // std::cout << response.returnMsg  << std::endl;
    return response;
}
