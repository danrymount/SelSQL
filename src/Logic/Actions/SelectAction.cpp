//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
#include "../../Parser/Headers/SelectVisitor.h"

Message SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor *>(getTreeVisitor().get());
    auto expr = v->getExpr();
    v->setExpressionVisitor(exprVisitor);
    auto source = v->getSource();
    source->accept(getTreeVisitor().get());
    auto message = v->getMessage();
    if (message.getErrorCode()) {
        commitTransaction(root);

        return message;
    }
    auto tableName = v->getTableName();
    if (tableName.empty()) {
        records = v->getRecords();
    } else {
        cursor = v->getEngine()->GetCursor(tableName, root->getId());
        auto table = cursor.first;
        auto columns = v->getColumns();
        std::vector<std::pair<std::string, std::string>> columnValues;

        for (auto &col : columns) {
            columnValues.emplace_back(std::make_pair(col.second, ""));
        }

        if (table->name.empty()) {
            commitTransaction(root);

            return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        }

        message = ActionsUtils::checkFieldsExist(table, columnValues);
        if (message.getErrorCode()) {
            commitTransaction(root);

            return message;
        }

        //        if (cursor.first->record_amount == 0) {
        //            return Message();
        //        }

        cursor.second->Reset();

        do {
            std::cerr << root->getId() << std::endl;
            auto _record = cursor.second->Fetch(root->getId());
            if (_record.empty()) {
                continue;
            }
            std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
            for (auto &col : _record) {
                _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
            }
            exprVisitor->setFirstValues(_newRecord);
            try {
                expr->accept(exprVisitor);
            } catch (std::exception &exception) {
                std::string exc = exception.what();
                return Message(ErrorConstants::ERR_TYPE_MISMATCH);
            }
            if (exprVisitor->getResult()) {
                records.push_back(_newRecord);
            }
        } while (!cursor.second->NextRecord());
    }
    v->setRecords(records);

    return Message(ActionsUtils::checkSelectColumns(records, v->getColumns()));
};