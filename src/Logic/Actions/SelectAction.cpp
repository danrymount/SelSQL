//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
#include "../../Parser/Headers/SelectVisitor.h"

Message SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    cursor = getEngine().GetCursor(root->getTableName());
    auto table = cursor.first;
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor *>(getTreeVisitor().get());
    auto columns = v->getColumns();
    auto expr = v->getExpr();
    std::vector<std::pair<std::string, std::string>> columnValues;
    columnValues.reserve(columns.size());
    for (auto &col : columns) {
        columnValues.emplace_back(std::make_pair(col, ""));
    }

    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    message = ActionsUtils::checkFieldsExist(cursor.first, columnValues);
    if (message.getErrorCode()) {
        return message;
    }

    if (cursor.first->record_amount == 0) {
        return Message(ActionsUtils::getTableInfo(table, 0));
    }

    cursor.second->Reset();
    do {
        auto _record = cursor.second->Fetch();
        if (_record.empty()) {
            continue;
        }
        v->setValues(_record);
        try {
            expr->accept(v);
        } catch (std::exception &exception) {
            std::string exc = exception.what();
            return Message(ErrorConstants::ERR_TYPE_MISMATCH);
        }

        if (v->getResult()) {
            records.push_back(_record);
        }
    } while (!cursor.second->Next());

    return Message(ActionsUtils::getTableInfo(table, 0) + ActionsUtils::checkSelectColumns(records, columns));
};