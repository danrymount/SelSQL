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
    response = mainEngine->Insert(&_request);
    requestToResponse(_request);

    return response;
}
