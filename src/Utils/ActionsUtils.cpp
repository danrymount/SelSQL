//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"
#include <queue>
#include <utility>

std::string ActionsUtils::makeRequestCreateFromTable(std::shared_ptr<Table> table) {
    const char space = ' ';
    const char semicolon = ';';
    const char comma = ',';
    std::string str = "CREATE TABLE ";
    str += table->name + '(';
    for (int i = 0; i < table->getFields().size(); ++i) {
        auto field = table->getFields()[i];
        str += field.first + space;
        str += ParserUtils::typeToString(field.second.type) + space;
        for (auto& constraint : field.second.getConstraints()) {
            str += ParserUtils::constraintToString(constraint) + space;
        }
        if (i != table->getFields().size() - 1 && table->getFields().size() > 1) {
            str += comma;
        }
    }
    str += ')';
    str += semicolon;

    return str;
}

int ActionsUtils::checkSameForUpdate(const Record& oldRec, const Record& newRec) {
    for (int i = 0; i < oldRec.size(); ++i) {
        if (oldRec[i].second != newRec[i].second) {
            return 0;
        }
    }
    return 1;
}

Error ActionsUtils::checkConstraint(std::vector<std::pair<std::string, std::string>> updateColumns,
                                    std::shared_ptr<Table> table, std::vector<ActionsUtils::Record> records,
                                    bool isUpdate) {
    Error error;
    std::string colName;
    int countSameVal = 0;
    for (auto& record : records) {
        int i = 0;
        for (auto& colValue : updateColumns) {
            auto value = colValue.second;
            if (colValue.first == "*") {
                colName = table->getFields()[i++].first;
            } else {
                colName = colValue.first;
            }

            //countSameVal += checkSameForUpdate(record, updateColumns); не сработает в случае, если update t set id = 1 where id = 1; гдк t = create table t(id INT UNIQUE, age INT); ,
            //могу быть записи (1, 0), а обновить на (1, 2) update t set id = 1, age = 2 where id = 1;

            for (auto& field : table->getFields()) {
                if (field.first != colName) {
                    continue;
                }

                for (auto& constraint : field.second.getConstraints()) {
                    if (record.empty()) {
                        if (constraint == Constraint::NOT_NULL || constraint == Constraint::PRIMARY_KEY) {
                            error = checkNotNull(value);
                            if (error.getErrorCode()) {
                                return error;
                            }
                        }
                        continue;
                    }

                    for (auto& elem : record) {
                        if (elem.first != colName) {
                            continue;
                        }
                        auto curVal = elem.second;
                        std::transform(curVal.begin(), curVal.end(), curVal.begin(),
                                       [](unsigned char c) { return std::tolower(c); });
                        error = constraintsCheckers[constraint](value, curVal);
                        if (error.getErrorCode() == ErrorConstants::ERR_UNIQUE && isUpdate) {
                            if(countSameVal > 1){
                                return error;
                            }
                        }else if (error.getErrorCode()) {
                            return error;
                        }
                    }
                }
            }
        }
    }
    return error;
}

Error ActionsUtils::checkNotNull(std::string newVal, std::string oldVal) {
    std::string temp = newVal;
    std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) { return std::tolower(c); });
    if (temp == "null") {
        return Error(ErrorConstants::ERR_NOT_NULL);
    }
    return Error();
}

Error ActionsUtils::checkUnique(std::string newVal, std::string oldVal) {
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

Error ActionsUtils::checkPrimaryKey(std::string newVal, std::string oldVal) {
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

Error ActionsUtils::checkFieldsExist(const std::shared_ptr<Table>& table,
                                     const std::vector<std::pair<std::string, std::string>>& updateColumns) {
    std::vector<int> existCols;
    for (auto& col : updateColumns) {
        //проверить для insert и update
        if (col.first == "*") {
            existCols.emplace_back(1);
            continue;
        }
        for (auto& tableCol : table->getFields()) {
            if (col.first == tableCol.first) {
                existCols.emplace_back(1);
            }
        }
    }
    if (existCols.size() != updateColumns.size()) {
        return Error(ErrorConstants::ERR_NO_SUCH_FIELD);
    }
    return Error();
}

void ActionsUtils::PrintSelect(std::vector<std::vector<std::pair<std::string, std::string>>> values) {
    if (values.empty()) {
        return;
    }
    std::string str;
    std::vector<int> len;
    std::stringstream stringstream;
    stringstream << std::endl;
    int n = values.size();
    int strSize = values[0].size();
    for (int i = 0; i < strSize; i++) {
        len.push_back(values[0][i].first.length());
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < strSize; j++) {
            int maxLen = len[j];
            values[i][j].second.length() > len[j] ? len[j] = values[i][j].second.length() : len[j] = len[j];
        }
    }
    for (int i = 0; i < strSize; i++) {
        stringstream << values[0][i].first;
        int lenStr = values[0][i].first.length();
        for (int j = lenStr; j < len[i]; j++) {
            stringstream << "\40";
        }
        stringstream << "|";
    }
    stringstream << std::endl;
    for (int i = 0; i < strSize; i++) {
        for (int j = 0; j < len[i]; j++) {
            stringstream << "-";
        }
        stringstream << "|";
    }
    stringstream << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < strSize; j++) {
            stringstream << values[i][j].second;
            int lenStr = values[i][j].second.length();
            for (int k = lenStr; k < len[j]; k++) {
                stringstream << "\40";
            }
            stringstream << "|";
        }
        stringstream << std::endl;
    }
    std::cout << stringstream.str();
}