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
    requestToResponse(_request);

    return response;
}
