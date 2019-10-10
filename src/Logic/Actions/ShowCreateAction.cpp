//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ShowCreateAction.h"
#include "../../Utils/Headers/ActionsUtils.h"

BigResponse ShowCreateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    ActionsUtils actionsUtils;
    requestToResponse(_request);
    response.ddlData.table = *mainEngine->ShowCreateTable(_request.tableName);
    if (&(response.ddlData.table) == nullptr) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }
    response.ddlData.returnMsg = actionsUtils.makeRequestCreateFromTable(response.ddlData.table);

    return response;
}
