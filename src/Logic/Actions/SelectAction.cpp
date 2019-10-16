//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
BigResponse SelectAction::execute(BigRequest& _request, MainEngine* mainEngine) {
    cursor = mainEngine->GetCursor(_request.tableName);

    if (cursor.first->name.empty()) {
        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return response;
    }
    std::map<std::string, Condition> cond = _request.dmlData.conditions;

    if (cursor.first->record_amount == 0) {
        return response;
    }
    do {
        auto _record = cursor.second->Fetch();
        if (_record.empty()) {
            continue;
        }
        if (cond.empty()) {
            response.dqlData.record.push_back(_record);
            continue;
        }
        for (auto field : _record) {
            std::string field_name = field.first;
            if (cond.find(field_name) != cond.end()) {
                if (ActionsUtils::check_condition(field.second, cond[field_name])) {
                    response.dqlData.record.push_back(_record);
                    continue;
                }
            }
        }

    } while (!cursor.second->Next());

    requestToResponse(_request);

    if (response.dqlData.columns.size() == 1 && response.dqlData.columns[0] == "*") {
        stringstream << " | ";
        std::cout << " | ";
        printAllHeader(cursor.first);
        std::cout << std::endl;
        stringstream << std::endl;
        printAll(response);
    } else {
        stringstream << " | ";
        std::cout << " | ";
        for (auto col : response.dqlData.columns) {
            if (col == "*") {
                printAllHeader(cursor.first);
                continue;
            }
            stringstream << col << " | ";
            std::cout << col << " | ";
        }
        std::cout << std::endl;
        stringstream << std::endl;

        for (auto record : response.dqlData.record) {
            stringstream << " | ";
            std::cout << " | ";
            for (auto col : response.dqlData.columns) {
                for (auto field : record) {
                    if (col == "*") {
                        printField(field.second);
                        continue;
                    } else if (col == field.first) {
                        printField(field.second);
                        break;
                    }
                }
            }
            stringstream << std::endl;
            std::cout << std::endl;
        }
    }

    response.select_message = stringstream.str();
    return response;
}
void SelectAction::printAll(BigResponse& response) {
    for (auto record : response.dqlData.record) {
        stringstream << " | ";
        std::cout << " | ";
        for (auto field : record) {
            std::cout << field.second << " | ";
            stringstream << field.second << " | ";
        }
        stringstream << std::endl;
        std::cout << std::endl;
    }
}
void SelectAction::printAllHeader(std::shared_ptr<Table> table) {
    for (auto col : table->getFields()) {
        std::cout << col.first << " | ";
        stringstream << col.first << " | ";
        ;
    }
}

void SelectAction::printField(std::string field) {
    std::cout << field << " | ";
    stringstream << field << " | ";
};