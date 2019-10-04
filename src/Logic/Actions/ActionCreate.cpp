//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionCreate.h"
Response ActionCreate::execute(Response &response) {
    errorCode = engineApi.CreateTable(&response.table);
    if (!errorCode)
        return response;
    std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
    response.code = errorCode;
    response.errorMsg = Constants::ERR_TABLE_EXISTS;
    return response;
}

// Utils/ActionsUtils.cpp Utils/Headers/ActionsUtils.h Parser/ParserLogic.cpp Parser/Headers/ParserLogic.h Logic/Actions/Headers/BaseAction.h Logic/Actions/Headers/ActionFactory.h Logic/Actions/ActionCreate.cpp Logic/Actions/Headers/ActionCreate.h Logic/Actions/ActionDrop.cpp Logic/Actions/Headers/ActionDrop.h Logic/Actions/ActionShowCreate.cpp Logic/Actions/Headers/ActionShowCreate.h
