//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionDrop.h"
BigResponse ActionDrop::execute(BigRequest& _request, MainEngine* mainEngine) {
    errorCode = mainEngine->DropTable(response.table.name);
    if (!errorCode)
        return response;
    // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
    response.error.errorCode = 1;
    response.error.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
    return BigResponse();
}
