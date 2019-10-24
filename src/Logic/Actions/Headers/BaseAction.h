//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_BASEACTION_H
#define SELSQL_BASEACTION_H

#include <utility>

#include "../../../Engine/Headers/MainEngine.h"
#include "../../../Parser/Nodes/ActionNodes/BaseActionNode.h"
#include "../../../Parser/Nodes/BaseNode.h"
#include "../../../Utils/Headers/ActionsUtils.h"
#include "../../../Utils/Structures/BigResponse.h"

class BaseAction {
   protected:
    int errorCode = 0;
    ActionsUtils actionsUtils;

   public:
    explicit BaseAction(std::shared_ptr<TreeVisitor> _visitor) : visitor(std::move(_visitor)) {}

    virtual Error execute(std::shared_ptr<BaseActionNode>) = 0;

    //virtual BigResponse execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) = 0;

    MainEngine getEngine() { return mainEngine; }

    std::shared_ptr<TreeVisitor> getTreeVisitor() { return visitor; }

    // move to Engine
    void requestToResponse(std::shared_ptr<BigRequest> _request) {
        response.action = _request->action;
        response.tableName = _request->tableName;
        if (response.dmlData.values.empty()) {
            response.dmlData.values = _request->dmlData.values;
            response.dmlData.columns = _request->dmlData.columns;
            response.dmlData.conditions = _request->dmlData.conditions;
        }

        if (response.dqlData.columns.empty()) {
            response.dqlData.conditions = _request->dqlData.conditions;
            response.dqlData.columns = _request->dqlData.columns;
            response.expression = _request->expression;
        }

        if (response.ddlData.table.name.empty()) {
            response.ddlData = _request->ddlData;
        }
    }
    BigResponse response;

   private:
    // shared ptr
    MainEngine mainEngine;
    std::shared_ptr<TreeVisitor> visitor;
};

#endif  // SELSQL_BASEACTION_H
