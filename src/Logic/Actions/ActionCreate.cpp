//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionCreate.h"
Response ActionCreate::execute(Response& response, MainEngine* mainEngine) {
    errorCode = mainEngine->CreateTable(&response.table);
    if (!errorCode)
        return response;
    // std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
    response.code = errorCode;
    response.errorMsg = Constants::ERR_TABLE_EXISTS;
    return response;
}