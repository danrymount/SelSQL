//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionShowCreate.h"
#include "../../Utils/Headers/ActionsUtils.h"

Response ActionShowCreate::execute(Response& response, MainEngine* mainEngine) {
    ActionsUtils actionsUtils;
    response.table = *mainEngine->ShowCreateTable(response.table.name);
    //response.table =
    if (&(response.table) == nullptr) {
        // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
        response.code = 1;
        response.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
        return response;
    }
    response.returnMsg = actionsUtils.makeRequestCreateFromTable(response.table);
    std::cout << response.returnMsg  << std::endl;
    return response;
}
