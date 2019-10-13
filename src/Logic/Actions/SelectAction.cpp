//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
BigResponse SelectAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;

    cursor = mainEngine->GetCursor(_request.tableName);

    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }

    std::vector<std::pair<std::string, std::string>> record;
    for (size_t i = 0; i < cursor.first->record_amount; ++i) {
        record = cursor.second->Fetch();
        for (auto field : record) {
            std::cout << field.first << " = " << field.second << std::endl;
        }
        if (cursor.second->Next()) {
            std::cout << "END OF DATA" << std::endl;
            break;
        }
    }

    requestToResponse(_request);

    return response;
}