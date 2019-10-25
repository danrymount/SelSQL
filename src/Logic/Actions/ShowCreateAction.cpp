//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ShowCreateAction.h"
#include "../../Utils/Headers/ActionsUtils.h"

//BigResponse ShowCreateAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    requestToResponse(_request);
//    response = mainEngine->ShowCreateTable(*_request.get());
//    if (response.error.getErrorCode()) {
//        return response;
//    }
//    response.ddlData.returnMsg = actionsUtils.makeRequestCreateFromTable(response.ddlData.table);
//
//    return response;
//}
Error ShowCreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    auto table = getEngine().ShowCreateTable(root->getTableName());
    if(table->name.empty()){
        return Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    //TODO make print for showcreate
    ActionsUtils::makeRequestCreateFromTable(table);
    return Error();
}
