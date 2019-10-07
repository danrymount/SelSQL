//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DropAction.h"
BigResponse DropAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    response.error.errorCode = mainEngine->DropTable(response.tableName);
    if (!response.error.errorCode)
        return response;
    // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
    response.error.errorCode = 1;
    response.error.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
    return BigResponse();
}
