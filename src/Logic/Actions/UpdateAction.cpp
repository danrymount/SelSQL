//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
BigResponse UpdateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    // response = mainEngine->Update(&_request);
    cursor = mainEngine->GetCursor(_request.tableName);
    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }

    response.error = ActionsUtils::checkFieldsExist(cursor.first, _request.dmlData.columns);
    if (response.error.getErrorCode()) {
        return response;
    }

    response.error = actionsUtils.checkConstraint(_request.dmlData.columns, _request.dmlData.values, cursor);
    if (response.error.getErrorCode()) {
        requestToResponse(_request);
        return response;
    }
    cursor.second->StartPos();

    auto expr = _request.expression;

    do {
        auto record = cursor.second->Fetch();
        // std::cout << cursor.second->current_pos << std::endl;
        if (expr.first.empty()) {
            cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);

        } else {
            RecordsData row;
            row.emplace_back(record);
            auto data = actionsUtils.checkExpression(expr, row);
            if (data.empty())
                continue;
            cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
        }

        //        response.error = actionsUtils.checkConstraint(_request.dmlData.columns, _request.dmlData.values,
        //        cursor); if (response.error.getErrorCode()) {
        //            requestToResponse(_request);
        //            return response;
        //        }
        //        cursor.second->StartPos();

    } while (!cursor.second->Next());

    cursor.second->Commit();

    return response;
}
