//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
BigResponse UpdateAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    // response = mainEngine->Update(&_request);
    requestToResponse(_request);

    return response;
}
