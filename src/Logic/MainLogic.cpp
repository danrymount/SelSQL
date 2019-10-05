//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include "Actions/Headers/ActionFactory.h"

Response MainLogic::executeRequest(Response &_response) {
    ActionFactory actionFactory;
    _response = actionFactory.create(_response.action)->execute(_response, mainEngine);
    return _response;
}