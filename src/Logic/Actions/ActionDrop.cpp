//
// Created by sapiest on 05.10.2019.
//

#include "Headers/ActionDrop.h"
Response ActionDrop::execute(Response &response) {
    errorCode = engineApi.DropTable(response.table.name);
    if(!errorCode)
        return response;
    // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
    response.code = 1;
    response.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
    return response;
}
