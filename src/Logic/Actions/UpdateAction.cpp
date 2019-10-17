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

    auto expr = _request.expression;

    do {
        auto record = cursor.second->Fetch();

        if (expr.first.empty()) {
            cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
        } else {
            RecordsData row;
            row.emplace_back(record);
            auto data = actionsUtils.checkExpression(expr, row);
            if (data.empty())
                continue;
            //            for (auto field: data[0]) {
            //                if()
            //            }
            //            auto t1 = data[0][0].first;
            //            auto t2 = data[0][0].second;
            cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
        }
        //            if (cond.empty()) {
        //                cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
        //            } else {

        //                if (cond.find(field_name) != cond.end()) {
        //                    if (ActionsUtils::checkSign(field.second, cond[field_name])) {
        //                        cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
        //                    }
        //                }
        //}

    } while (!cursor.second->Next());

    cursor.second->Commit();

    return response;
}
