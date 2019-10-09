//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DeleteAction.h"
BigResponse DeleteAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    response = mainEngine->Delete(&_request);
    return response;
}
