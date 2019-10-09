//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
BigResponse SelectAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    response = mainEngine->Select(&_request);
    return response;
}
