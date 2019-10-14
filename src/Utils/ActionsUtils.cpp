//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"

std::string ActionsUtils::makeRequestCreateFromTable(Table table) {
    const char space = ' ';
    const char semicolon = ';';
    const char comma = ',';
    std::string str = "CREATE TABLE ";
    str += table.name + '(';
    for (int i = 0; i < table.getFields().size(); ++i) {
        auto field = table.getFields()[i];
        str += field.first + space;
        str += parserUtils.typeToString(field.second.type) + space;
        for (auto& constraint : field.second.getConstraints()) {
            str += parserUtils.constraintToString(constraint) + space;
        }
        if (i != table.getFields().size() - 1 && table.getFields().size() > 1) {
            str += comma;
        }
    }
    str += ')';
    str += semicolon;

    return str;
}

Error ActionsUtils::checkConstraint(std::vector<std::string> columns, std::vector<std::string> values,
                                    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor) {
    std::string val;
    Error error;
    std::shared_ptr<Table> table = cursor.first;

    auto record = cursor.second->Fetch();
    do {
        /// for rows data
        for (int i = 0; i < table->getFields().size(); ++i) {
            auto tableCol = table->getFields()[i];
            if (columns.empty()) {
                val = values[i];
            } else {
                auto valIt = std::find(columns.begin(), columns.end(), tableCol.first);
                if (valIt == columns.end()) {
                    val = "null";
                } else {
                    int columnIndex = std::distance(columns.begin(), valIt);
                    val = values[columnIndex];
                }
            }

            for (int j = 0; j < tableCol.second.getConstraints().size(); ++j) {
                auto id = tableCol.second.getConstraints()[j];
                if (record.empty()) {
                    if (!id || id == PRIMARY_KEY) {
                        error = checkNotNull(val, val);
                        if (error.getErrorCode()) {
                            return error;
                        }
                    }
                }
                for (auto& elem : record) {
                    if (tableCol.first == elem.first) {
                        auto curVal = elem.second;
                        error = constraintsCheckers[id](val, curVal);
                        if (error.getErrorCode()) {
                            return error;
                        }
                        break;
                    }
                }
            }
        }
        record = getTableRecord(cursor);
    } while (!record.empty());

    return error;
}

Error ActionsUtils::checkNotNull(const std::string& newVal, const std::string& oldVal) {
    if (newVal == "null") {
        return Error(ErrorConstants::ERR_NOT_NULL);
    }
    return Error();
}

Error ActionsUtils::checkUnique(const std::string& newVal, const std::string& oldVal) {
    std::string val;
    if (newVal[0] == '\'') {
        val = newVal.substr(1, newVal.length() - 1);
    } else {
        val = newVal;
    }
    if (val == oldVal) {
        return Error(ErrorConstants::ERR_UNIQUE);
    }
    return Error();
}

Error ActionsUtils::checkPrimaryKey(const std::string& newVal, const std::string& oldVal) {
    Error err = checkNotNull(newVal, oldVal);
    if (err.getErrorCode()) {
        return err;
    }
    err = checkUnique(newVal, oldVal);
    if (err.getErrorCode()) {
        return err;
    }
    return Error();
}

ActionsUtils::Record ActionsUtils::getTableRecord(std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor) {
    Record record;
    if (!cursor.second->Next()) {
        return cursor.second->Fetch();
    }
    return record;

    //    for (size_t i = 0; i < cursor.first->record_amount; ++i) {
    //        record = cursor.second->Fetch();
    //        for (auto field : record) {
    //            std::cout << field.first << " = " << field.second << std::endl;
    //        }
    //        if (cursor.second->Next()) {
    //            std::cout << "END OF DATA" << std::endl;
    //            break;
    //        }
    //    }
}
