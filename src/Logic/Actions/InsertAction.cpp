//
// Created by sapiest on 05.10.2019.
//

#include "Headers/InsertAction.h"
#include "../../Parser/Headers/InsertVisitor.h"

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

    if (columns[0] == "*" and (values.size() != cursor.first->getFields().size())) {
        return Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
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
    std::vector<ActionsUtils::Record> records;
    if (cursor.first->record_amount) {
        records = ActionsUtils::getAllRecords(cursor);
    }

    message = ActionsUtils::checkFieldsExist(table, columnsValues);
    if (message.getErrorCode()) {
        return message;
    }

    message = actionsUtils.checkConstraint(columnsValues, cursor.first, records);
    if (message.getErrorCode()) {
        return message;
    }

    try {
        cursor.second->Insert(newCols, values);
    } catch (std::exception &exception) {
        return Message(ErrorConstants::ERR_STO);
    }

    cursor.second->Reset();

    return message;
}
