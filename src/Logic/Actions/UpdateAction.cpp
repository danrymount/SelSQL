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

    std::map<std::string, Condition> cond = _request.dmlData.conditions;

    do {
        auto record = cursor.second->Fetch();

        for (auto field : record) {
            std::string field_name = field.first;
            if (cond.empty()) {
                cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
            } else {
                if (cond.find(field_name) != cond.end()) {
                    if (ActionsUtils::check_condition(field.second, cond[field_name])) {
                        cursor.second->Update(_request.dmlData.columns, _request.dmlData.values);
                    }
                }
            }
        }

    } while (!cursor.second->Next());

    cursor.second->Commit();

    return response;
}
