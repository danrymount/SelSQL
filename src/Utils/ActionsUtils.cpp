//
// Created by sapiest on 02.10.2019.
//

#include "Headers/ActionsUtils.h"
#include <queue>

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
int ActionsUtils::checkSign(std::string rec_val, Condition cond_val) {
    int res = 0;
    switch (cond_val.sign) {
        case GREATEREQUALS:
            if (rec_val >= cond_val.value) {
                res = 1;
            }
            break;
        case GREATER:
            if (rec_val > cond_val.value) {
                res = 1;
            }
            break;
        case NOEQUALS:
            if (rec_val != cond_val.value) {
                res = 1;
            }
            break;
        case EQUALS: {
            if (rec_val == cond_val.value) {
                res = 1;
            }
            break;
        }
        case LOWER:
            if (rec_val < cond_val.value) {
                res = 1;
            }
            break;
        case LOWEREQUALS:
            if (rec_val <= cond_val.value) {
                res = 1;
            }
            break;
    }
    return res;
}
RecordsData ActionsUtils::checkExpression(std::pair<Expr, vecString> expr, RecordsData recordsData) {
    RecordsData newRecords;
    std::vector<std::pair<std::string, Condition>> exprRes;
    for (auto& row : recordsData) {
        for (auto& record : row) {
            std::vector<std::pair<Cmp, std::string>> res = countExpr(record.first, record.second, expr.first);
            if (res.empty()) continue;
            for (auto& exp : res) {
                exprRes.emplace_back(std::make_pair(record.first, Condition(exp.first, exp.second)));
            }
        }
    }
    return newRecords;
}

std::vector<std::pair<Cmp, std::string>> ActionsUtils::countExpr(std::string columnName, std::string val, Expr exprs) {
    std::vector<std::pair<Cmp, std::string>> res;
    std::queue<double> curExpr;
    for (int index = 0; index < exprs.size(); index++) {
        auto expr = exprs[index];
        // for (auto& expr : exprs) {
        //        if (expr.first.first.second) {
        //            continue;
        //        }

        auto valExpr = expr.first.first;  //название колонки в экспрешоне
        if (valExpr != columnName) continue;
        // expr.first.first = "";
        // expr.first.first.second = 1;
        //        if (std::isdigit(*val.c_str())) {
        //            auto cmp = expr.first.second;
        //            auto tempExpr = expr.second;
        //            if (tempExpr.empty()) {
        //                continue;
        //            }
        //            int i = 0;
        //            while (i != tempExpr.size()) {
        //                auto elem = tempExpr[i];
        //                i++;
        //                if (elem == columnName) {
        //                    elem = val;
        //                }
        //
        //                if (std::isdigit(*elem.c_str())) {
        //                    curExpr.push(std::stoi(elem));
        //                    continue;
        //                }
        //                double a = curExpr.front();
        //                curExpr.pop();
        //                double b = curExpr.front();
        //                curExpr.pop();
        //                curExpr.push(ActionsUtils::calculate[elem](a, b));
        //            }
        //            res.emplace_back(std::make_pair(cmp, std::to_string(curExpr.front())));
        //            // expr.first.first = "";
        //            // expr.first.first.second = 1;
        //        }
        //        } else {
        //            auto curExprVal = expr.second[0].substr(1, expr.second[0].length() - 2);
        //            // auto valExpr = valExpr.substr(1, valExpr.length() - 3);
        //            auto cmp = expr.first.second;
        //            res.emplace_back(std::make_pair(cmp, curExprVal));
        //        }
    }
    return res;

    //            if (vectorSigns.empty()) {
    //                if(sign == "*" || sign == "/")
    //                    curExpr.push(sign);
    //                else
    //                    vectorSigns.push_back(sign);
    //                i++;
    //            } else {
    //                auto priority = checkPriority[sign];
    //                auto oldPriority = checkPriority[tempExpr[i - 1]];
    //                if (oldPriority == 0 && priority > oldPriority) {
    //                    vectorSigns.push_back(sign);
    //                    i++;
    //                } else if (sign == ")") {
    //                    int k = vectorSigns.size();
    //                    while (vectorSigns[--k] != "(") {
    //                        curExpr.push(vectorSigns[k]);
    //                            vectorSigns.pop_back();
    //                        }
    //                        vectorSigns.pop_back();
    //                        i++;
    //                } else {
    //                    curExpr.push(vectorSigns.front());
    //                        vectorSigns.pop_front();
    //                    i++;
    //                }

    //        if (!tempExpr.first.empty()) {
    //            int i = 0;
    //            int j = 0;
    //            while (tempExpr.first.size() != 1) {
    //                if (tempExpr.first[i] == columnName) {
    //                    tempExpr.first[i] = val;
    //                }
    //                vectorNums.push(std::stoi(tempExpr.first[i]));
    //                if (vectorNums.size() < 2) {
        //                    i++;
        //                    continue;
        //                }
        //                if (vectorSigns.empty()) {
        //                    vectorSigns.push(tempExpr.second[j]);
        //                    j++;
        //                    continue;
        //                }
        //                auto sign = tempExpr.second[j];
        //                auto priority = checkPriority[sign];
        //                auto oldPriority = checkPriority[tempExpr.second[j - 1]];
        //                if (oldPriority != 4 && priority > oldPriority) {
        //                    vectorSigns.push(sign);
        //                    j++;
        //                    continue;
        //                } else if (oldPriority == priority && oldPriority == 4) {
        //                    auto a = vectorNums.front();
        //                    vectorNums.pop();
        //                    auto b = vectorNums.front();
        //                    vectorNums.pop();
        //                    result.push(calculate[sign](a, b));
        //                    j++;
        //                } else {
        //                    auto a = vectorNums.front();
        //                    vectorNums.pop();
        //                    auto b = vectorNums.front();
        //                    vectorNums.pop();
        //                    result.push(calculate[sign](a, b));
        //                    j++;
        //                }
        //            }
        //            res.second = tempExpr.first[0];
        //        }
    //}
}