//
// Created by sapiest on 05.10.2019.
//

#include "Headers/CreateAction.h"
//BigResponse CreateAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    response = mainEngine->CreateTable(_request.get());
//    requestToResponse(_request);
//    if (!response.error.getErrorCode())
//        return response;
//    // std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
//    response.error = Error(ErrorConstants::ERR_TABLE_EXISTS);
//
//    return response;
//}
Error CreateAction::execute(std::shared_ptr<BaseActionNode>) { return Error(); }
