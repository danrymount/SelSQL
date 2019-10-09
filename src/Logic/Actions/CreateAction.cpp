//
// Created by sapiest on 05.10.2019.
//

#include "Headers/CreateAction.h"
BigResponse CreateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    int error = mainEngine->CreateTable(&_request.ddlData.table);
    if (!error)
        return response;
    // std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
    response.error = Error(ErrorConstants::ERR_TABLE_EXISTS);
    return response;
}