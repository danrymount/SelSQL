//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DeleteAction.h"

BigResponse DeleteAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    cursor = mainEngine->GetCursor(_request.tableName);
    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }

    int delete_count = 0;

    std::map<std::string, Condition> cond = _request.dmlData.conditions;

    do {
        auto record = cursor.second->Fetch();
        if (cond.empty()) {
            delete_count++;
            cursor.second->Delete();
            continue;
        }
        for (auto field : record) {
            std::string field_name = field.first;
            if (cond.find(field_name) != cond.end()) {
                if (ActionsUtils::check_condition(field.second, cond[field_name])) {
                    delete_count++;
                    cursor.second->Delete();
                }
            }
        }

    } while (!cursor.second->Next());

    cursor.second->table->record_amount -= delete_count;
    cursor.second->Commit();
    return response;
}
