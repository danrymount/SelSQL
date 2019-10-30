//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
#include "../../Parser/Headers/SelectVisitor.h"

Message SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor *>(getTreeVisitor().get());
    // auto columns = v->getColumns();
    auto expr = v->getExpr();
    auto source = v->getSource();
    source->accept(getTreeVisitor().get());
    auto message = v->getMessage();
    if (message.getErrorCode()) {
        return message;
    }
    auto tableName = v->getTableName();
    if (tableName.empty()) {
        records = v->getRecords();
    } else {
        cursor = getEngine().GetCursor(tableName);
        auto table = cursor.first;
        auto columns = v->getColumns();
        std::vector<std::pair<std::string, std::string>> columnValues;

        for (auto &col : columns) {
            columnValues.emplace_back(std::make_pair(col.second, ""));
        }

        if (table->name.empty()) {
            return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        }

        message = ActionsUtils::checkFieldsExist(cursor.first, columnValues);
        if (message.getErrorCode()) {
            return message;
        }

        if (cursor.first->record_amount == 0) {
            //        cursor.second.reset();
            return Message(ActionsUtils::getTableInfo(table, 0));
        }


        cursor.second->Reset();
        do {
            auto _record = cursor.second->Fetch();
            if (_record.empty()) {
                continue;
            }
            std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
            for (auto &col : _record) {
                _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
            }
            v->setFirstValues(_newRecord);
            expr->accept(v);
            if (v->getResult()) {
                records.push_back(_newRecord);
            }
        } while (!cursor.second->Next());
    }
    return Message(ActionsUtils::checkSelectColumns(records, v->getColumns()));
};