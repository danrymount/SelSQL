//
// Created by sapiest on 05.10.2019.
//

#include "Headers/InsertAction.h"
BigResponse InsertAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    if ((_request.dmlData.values.size() != _request.dmlData.columns.size()) && (!_request.dmlData.columns.empty())) {
        response.error = Error(ErrorConstants::ERR_INSERT_VALUES_SIZE);
        return response;
    }
    for (int i = 0; i < _request.dmlData.columns.size(); ++i) {
        auto col = _request.dmlData.columns[i];
        for (int j = 0; j < _request.dmlData.columns.size(); ++j) {
            if (i == j)
                continue;
            if (col == _request.dmlData.columns[j]) {
                response.error = Error(ErrorConstants::ERR_SAME_COLUMN);
                requestToResponse(_request);
                return response;
            }
        }
    }
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    cursor = mainEngine->GetCursor(_request.tableName);

    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }

    cursor.second->Insert(_request.dmlData.columns, _request.dmlData.values);
    cursor.second->Commit();
    //    cursor = mainEngine->GetCursor(_request.tableName);
    //    for (auto i :cursor.second->Fetch()){
    //        std::cout<<i.first<<" == "<<i.second<<std::endl;
    //    }
    requestToResponse(_request);

    return response;
}
