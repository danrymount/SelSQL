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

    auto expr = _request.expression;

    do {
        auto record = cursor.second->Fetch();
        if (expr.first.empty()) {
            delete_count++;
            cursor.second->Delete();
            continue;
        } else {
            RecordsData row;
            row.emplace_back(record);
            auto data = actionsUtils.checkExpression(expr, row);
            if (data.empty())
                continue;
            delete_count++;
            cursor.second->Delete();
        }

    } while (!cursor.second->Next());

    cursor.second->table->record_amount -= delete_count;
    cursor.second->Commit();
    return response;
}
