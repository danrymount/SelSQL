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
        if (response.dmlData.values.empty()) {
            response.dmlData.values = _request.dmlData.values;
            response.dmlData.columns = _request.dmlData.columns;
            response.dmlData.conditions = _request.dmlData.conditions;
        }

        if (response.dqlData.columns.empty()) {
            response.dqlData.conditions = _request.dqlData.conditions;
            response.dqlData.columns = _request.dqlData.columns;
            response.expression = _request.expression;
        }

        if (response.ddlData.table.name.empty()) {
            response.ddlData = _request.ddlData;
        }
    }
    BigResponse response;
};

#endif  // SELSQL_BASEACTION_H
