//
// Created by sapiest on 05.10.2019.
//

#include "Headers/InsertAction.h"
#include "../../Parser/Headers/InsertVisitor.h"

Message InsertAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<InsertVisitor *>(getTreeVisitor().get());
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

    cursor = v->getEngine()->GetCursor(v->getTableName());

    auto table = cursor.first;
    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    if (!columns.empty() && columns[0] == "*" && (values.size() != cursor.first->getFields().size())) {
        return Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
    }

    //    if (table->record_amount == Constants::DATA_SIZE / table->record_size) {
    //        return Message(ErrorConstants::ERR_TABLE_FULL);
    //    }

    if (columns.empty() && (table->getFields().size() != values.size())) {
        return Message(ErrorConstants::ERR_INSERT_VALUES_SIZE);
    }

    std::vector<std::pair<std::string, std::string>> columnsValues;

    std::vector<int> checkExists;
    if (!columns.empty() && columns[0] != "*") {
        for (auto &field : table->getFields()) {
            for (auto &col : columns) {
                if (field.first == col) {
                    checkExists.emplace_back(1);
                }
            }
        }

        if (checkExists.size() != columns.size()) {
            return Message(ErrorConstants::ERR_NO_SUCH_FIELD);
        }
    }

    for (int i = 0; i < table->getFields().size(); i++) {
        std::pair<std::string, std::string> curColValue;
        auto tableFiels = cursor.first->getFields()[i];

        std::string val = "";
        if (!columns.empty()) {
            if (columns[0] == "*") {
                if (tableFiels.second.type == Type::TYPE_CHAR) {
                    val = values[i].substr(1, values[i].length() - 2);
                } else {
                    val = values[i];
                }
                if (!tableFiels.second.size || tableFiels.second.size >= val.length()) {
                    columnsValues.emplace_back(std::make_pair(cursor.first->getFields()[i].first, values[i]));
                    continue;
                } else {
                    return Message(ErrorConstants::ERR_BIG_CHAR);
                }
            }
        }
        auto it = std::find(columns.begin(), columns.end(), tableFiels.first);
        if (it != columns.end()) {
            auto index = std::distance(columns.begin(), it);
            if (tableFiels.second.type == Type::TYPE_CHAR) {
                val = values[i].substr(1, values[index].length() - 2);
            } else {
                val = values[i];
            }
            if (!tableFiels.second.size || tableFiels.second.size >= val.length()) {
                columnsValues.emplace_back(std::make_pair(columns[index], values[index]));
            }
        } else {
            columnsValues.emplace_back(std::make_pair(cursor.first->getFields()[i].first, ""));
        }
    }
    std::vector<ActionsUtils::Record> records;
    //    if (cursor.first->record_amount) {
    records = ActionsUtils::getAllRecords(cursor);
    //    }

    message = ActionsUtils::checkFieldsExist(table, columnsValues);
    if (message.getErrorCode()) {
        return message;
    }

    message = actionsUtils.checkConstraint(columnsValues, cursor.first, records);
    if (message.getErrorCode()) {
        return message;
    }

    try {
        std::vector<std::string> newCols;
        std::vector<std::string> newVals;
        for (auto &colVal : columnsValues) {
            newCols.emplace_back(colVal.first);
            newVals.emplace_back(colVal.second);
        }
        if (cursor.second->Insert(newCols, newVals, root->getId()) == ErrorConstants::ERR_TRANSACT_CONFLICT) {
            return Message(ErrorConstants::ERR_TRANSACT_CONFLICT);
        };
    } catch (std::exception &exception) {
        return Message(ErrorConstants::ERR_STO);
    }

    cursor.second->Commit();
    

    return message;
}
