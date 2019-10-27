//
// Created by sapiest on 05.10.2019.
//

#include "Headers/InsertAction.h"
#include "../../Parser/Headers/InsertVisitor.h"
// BigResponse InsertAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    auto columns = _request->dmlData.columns;
//    auto values = _request->dmlData.values;
//    if ((values.size() != columns.size()) && (!columns.empty())) {
//        response.error = Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
//
//        return response;
//    }
//
//    for (int i = 0; i < columns.size(); ++i) {
//        auto col = columns[i];
//        for (int j = 0; j < columns.size(); ++j) {
//            if (i == j)
//                continue;
//            if (col == columns[j]) {
//                response.error = Message(ErrorConstants::ERR_SAME_COLUMN);
//                requestToResponse(_request);
//
//                return response;
//            }
//        }
//    }
//
//    cursor = mainEngine->GetCursor(_request->tableName);
//
//    if (cursor.first->name.empty()) {
//        response.error = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
//
//        return response;
//    }
//
//    if (cursor.first->record_amount == Constants::DATA_BLOCK_SIZE / cursor.first->record_size) {
//        response.error = Message(ErrorConstants::ERR_TABLE_FULL);
//        return response;
//    }
//
//    std::shared_ptr<Table> table = cursor.first;
//
//    if (columns.empty() && (table->getFields().size() != values.size())) {
//        response.error = Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
//
//        return response;
//    }
//
//    response.error = ActionsUtils::checkFieldsExist(table, columns);
//    if (response.error.getErrorCode()) {
//        return response;
//    }
//
//    // response.dqlData.record = actionsUtils.getTableRecord(cursor);
//    response.error = actionsUtils.checkConstraint(columns, values, cursor);
//    if (response.error.getErrorCode()) {
//        requestToResponse(_request);
//        return response;
//    }
//
//    cursor.second->Insert(columns, _request->dmlData.values);
//    cursor.second->Commit();
//    //    cursor = mainEngine->GetCursor(_request.tableName);
//    //    for (auto i :cursor.second->Fetch()){
//    //        std::cout<<i.first<<" == "<<i.second<<std::endl;
//    //    }
//
//    return response;
//}

Message InsertAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<InsertVisitor*>(getTreeVisitor().get());
    auto columns = v->getColumns();
    auto values = v->getValues();

    if ((values.size() != columns.size()) && !(columns.size() == 1 && columns[0] == "*")) {
        return Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
    }

    for (int i = 0; i < columns.size(); ++i) {
        auto col = columns[i];
        for (int j = 0; j < columns.size(); ++j) {
            if (i == j)
                continue;
            if (col == columns[j]) {
                return Message(ErrorConstants::ERR_SAME_COLUMN);
            }
        }
    }

    cursor = getEngine().GetCursor(root->getTableName());

    auto table = cursor.first;
    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    //    if (table->record_amount == Constants::DATA_SIZE / table->record_size) {
    //        return Message(ErrorConstants::ERR_TABLE_FULL);
    //    }

    if (columns.empty() && (table->getFields().size() != values.size())) {
        return Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
    }

    std::vector<std::pair<std::string, std::string>> columnsValues;
    std::vector<std::string> newCols;
    for (int i = 0; i < table->getFields().size(); i++) {
        std::pair<std::string, std::string> curColValue;
        if (columns[0] == "*") {
            columnsValues.emplace_back(std::make_pair(cursor.first->getFields()[i].first, values[i]));
            newCols.emplace_back(cursor.first->getFields()[i].first);
            continue;
        }
        if (i < values.size()) {
            columnsValues.emplace_back(std::make_pair(columns[i], values[i]));
        } else {
            columnsValues.emplace_back(std::make_pair(cursor.first->getFields()[i].first, "null"));
        }
    }

    std::vector<ActionsUtils::Record> records = ActionsUtils::getAllRecords(cursor);

    error = ActionsUtils::checkFieldsExist(table, columnsValues);
    if (error.getErrorCode()) {
        return error;
    }

    error = actionsUtils.checkConstraint(columnsValues, cursor.first, records);
    if (error.getErrorCode()) {
        return error;
    }

    cursor.second->Insert(newCols, values);
    //    cursor.second->Commit();

    cursor.second->StartPos();

    return error;
}
