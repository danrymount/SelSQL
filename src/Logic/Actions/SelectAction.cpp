//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
BigResponse SelectAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    cursor = mainEngine->GetCursor(_request.tableName);

    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }

    auto record = cursor.second->Fetch();
    do {
        response.dmlData.record.push_back(record);
        for (auto field : record) {
            std::cout << field.first << " = " << field.second << std::endl;
        }
        record = actionsUtils.getTableRecord(cursor);
    } while (!record.empty());

    requestToResponse(_request);

    return response;
}