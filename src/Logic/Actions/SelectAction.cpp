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


Error SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    cursor = getEngine().GetCursor(root->getTableName());
    auto table = cursor.first;
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor*>(getTreeVisitor().get());
    auto columns = v->getColumns();
    auto expr = v->getExpr();
    std::vector<std::pair<std::string, std::string>> columnValues;
    for (auto &col: columns) {
        columnValues.emplace_back(std::make_pair(col, ""));
    }

    if (table->name.empty()) {
        return Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    error = ActionsUtils::checkFieldsExist(cursor.first, columnValues);
    if (error.getErrorCode()) {
        return error;
    }

    if (cursor.first->record_amount == 0) {
        cursor.second.reset();
        return error;
    }

    cursor.second->StartPos();
    do {
        auto _record = cursor.second->Fetch();
        if (_record.empty()) {
            continue;
        }
        v->setValues(_record);
        expr->accept(v);
        if(v->getResult()){
            records.push_back(_record);
        }
    } while (!cursor.second->Next());

    ActionsUtils::PrintSelect(records);

    cursor.second->StartPos();

    return Error();
};