//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
#include "../../Parser/Headers/SelectVisitor.h"
// BigResponse SelectAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    cursor = mainEngine->GetCursor(_request->tableName);
//
//    if (cursor.first->name.empty()) {
//        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
//        return response;
//    }
//
//    response.error = ActionsUtils::checkFieldsExist(cursor.first, _request->dqlData.columns);
//    if (response.error.getErrorCode()) {
//        return response;
//    }
//
//    if (cursor.first->record_amount == 0) {
//        return response;
//    }
//    do {
//        auto _record = cursor.second->Fetch();
//        if (_record.empty()) {
//            continue;
//        }
//        response.dqlData.record.push_back(_record);
//
//    } while (!cursor.second->Next());
//
//    requestToResponse(_request);
//
//    if (!response.expression.first.empty()) {
//        response.dqlData.record = actionsUtils.checkExpression(response.expression, response.dqlData.record);
//    }
//
//    if (response.dqlData.columns.size() == 1 && response.dqlData.columns[0] == "*") {
//        stringstream << " | ";
//        std::cout << " | ";
//        printAllHeader(cursor.first);
//        std::cout << std::endl;
//        stringstream << std::endl;
//        printAll(response);
//    } else {
//        stringstream << " | ";
//        std::cout << " | ";
//        for (auto col : response.dqlData.columns) {
//            if (col == "*") {
//                printAllHeader(cursor.first);
//                continue;
//            }
//            stringstream << col << " | ";
//            std::cout << col << " | ";
//        }
//        std::cout << std::endl;
//        stringstream << std::endl;
//
//        for (auto record : response.dqlData.record) {
//            stringstream << " | ";
//            std::cout << " | ";
//            for (auto col : response.dqlData.columns) {
//                for (auto field : record) {
//                    if (col == "*") {
//                        printField(field.second);
//                        continue;
//                    } else if (col == field.first) {
//                        printField(field.second);
//                        break;
//                    }
//                }
//            }
//            stringstream << std::endl;
//            std::cout << std::endl;
//        }
//    }
//
//    response.select_message = stringstream.str();
//    return response;
//}

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
    }
}

void SelectAction::printField(std::string field) {
    std::cout << field << " | ";
    stringstream << field << " | ";
}
Error SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor*>(getTreeVisitor().get());
    auto res = v->getResult();
    std::cout << res << std::endl;
    auto columns = v->getColumns();
    return Error();
};