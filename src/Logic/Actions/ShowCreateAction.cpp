//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ShowCreateAction.h"
#include "../../Utils/Headers/ActionsUtils.h"

BigResponse ShowCreateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    ActionsUtils actionsUtils;
    requestToResponse(_request);
    response = mainEngine->ShowCreateTable(_request);
    if (response.error.getErrorCode()) {
        return response;
    }
    response.ddlData.returnMsg = actionsUtils.makeRequestCreateFromTable(response.ddlData.table);

    return response;
}
