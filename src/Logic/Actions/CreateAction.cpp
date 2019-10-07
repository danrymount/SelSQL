//
// Created by sapiest on 05.10.2019.
//

#include "Headers/CreateAction.h"
BigResponse CreateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    response.error.errorCode = mainEngine->CreateTable(&_request.ddlData.table);
    if (!response.error.errorCode)
        return response;
    // std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
    response.error.errorCode = errorCode;
    response.error.errorMsg = Constants::ERR_TABLE_EXISTS;
    return BigResponse();
}