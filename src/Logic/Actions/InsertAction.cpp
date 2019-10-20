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

    cursor = mainEngine->GetCursor(_request.tableName);

    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);

        return response;
    }

    if (cursor.first->record_amount == Constants::DATA_BLOCK_SIZE / cursor.first->record_size) {
        response.error = Error(ErrorConstants::ERR_TABLE_FULL);
        return response;
    }

    //    std::shared_ptr<Table> table = cursor.first;
    //
    //    if (_request.dmlData.columns.empty() && (table->getFields().size() != _request.dmlData.values.size())) {
    //        response.error = Error(ErrorConstants::ERR_INSERT_VALUES_SIZE);
    //
    //        return response;
    //    }
    //
    //    response.error = ActionsUtils::checkFieldsExist(table, _request.dmlData.columns);
    //    if (response.error.getErrorCode()) {
    //        return response;
    //    }
    //
    //    // response.dqlData.record = actionsUtils.getTableRecord(cursor);
    //    response.error = actionsUtils.checkConstraint(_request.dmlData.columns, _request.dmlData.values, cursor);
    //    if (response.error.getErrorCode()) {
    //        requestToResponse(_request);
    //        return response;
    //    }

    cursor.second->Insert(_request.dmlData.columns, _request.dmlData.values);
    cursor.second->Commit();
    //    cursor = mainEngine->GetCursor(_request.tableName);
    //    for (auto i :cursor.second->Fetch()){
    //        std::cout<<i.first<<" == "<<i.second<<std::endl;
    //    }

    return response;
}
