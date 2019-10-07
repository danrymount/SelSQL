//
// Created by sapiest on 05.10.2019.
//

#include "Headers/InsertAction.h"
BigResponse InsertAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    if ((_request.dmlData.values.size() != _request.dmlData.columns.size()) && (!_request.dmlData.columns.empty())) {
        response.error = Error(ErrorConstants::ERR_INSERT_VALUES_SIZE);
        return response;
    }
    response = mainEngine->Insert(&_request);
    // if (!response.error.errorCode)
    return response;
    // maybe make 2 functions with different args
    // mainEngine->InsertIntoTableWithCols(response.table.name, response) TODO (tableName, map<string, Any> colsValues)
    // mainEngine->InsertIntoTable(response.table.name, response) TODO (tableName, vector<Any> values)
    // or make 1 function
    // mainEngine->InsertIntoTable(tableName, vector<string> colsNames = null, vector<Any> colsValues )
    // if (colsNames == null or empty) it means, that we want to insert element in table, not in column
    // EX INSERT INTO TABLE test1 VALUES(0, "name", false);
    // else { EX INSERT INTO TABLE test1(name) VALUES ("kek); } if it can be null or has default value
}
