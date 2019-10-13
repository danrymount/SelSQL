//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_BASEACTION_H
#define SELSQL_BASEACTION_H

#include "../../../Engine/Headers/MainEngine.h"
#include "../../../Utils/Headers/ActionsUtils.h"
#include "../../../Utils/Structures/BigResponse.h"

class BaseAction {
   protected:
    int errorCode;
    ActionsUtils actionsUtils;

   public:
    virtual BigResponse execute(BigRequest& _request, MainEngine* mainEngine) = 0;

    // move to Engine
    void requestToResponse(BigRequest& _request) {
        response.action = _request.action;
        response.tableName = _request.tableName;
        response.dmlData = _request.dmlData;
        response.dqlData = _request.dqlData;
        response.ddlData = _request.ddlData;
    }
    BigResponse response;
};

#endif  // SELSQL_BASEACTION_H
