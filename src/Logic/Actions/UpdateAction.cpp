//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
#include "../../Parser/Headers/UpdateVisitor.h"

Message UpdateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<UpdateVisitor *>(getTreeVisitor().get());
    auto updateColumns = v->getUpdates();
    auto expr = v->getExpr();
    cursor = getEngine().GetCursor(v->getTableName());
    auto table = cursor.first;
    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    message = ActionsUtils::checkFieldsExist(table, updateColumns);
    if (message.getErrorCode()) {
        return message;
    }

    std::vector<ActionsUtils::Record> records;
    std::vector<ActionsUtils::Record> allrecords;
    // = ActionsUtils::getAllRecords(cursor);
    // cursor.second->Reset();
    if (cursor.first->record_amount) {
        do {
            auto record = cursor.second->Fetch();
            if (record.empty()) {
                continue;
            }
            v->setValues(record);
            try {
                expr->accept(getTreeVisitor().get());
            } catch (std::exception &exception) {
                std::string exc = exception.what();
                return Message(ErrorConstants::ERR_TYPE_MISMATCH);
            }
            if (v->getResult()) {
                records.emplace_back(record);
            }
            allrecords.emplace_back(record);
        } while (!cursor.second->NextRecord());
        cursor.second->Reset();

        // TODO сменить входные параметры
        std::vector<std::string> columns;
        std::vector<std::string> values;
        for (auto &colValue : updateColumns) {
            columns.emplace_back(colValue.first);
            values.emplace_back(colValue.second);
        }

        message = actionsUtils.checkConstraintFroUpdate(updateColumns, cursor.first, records, allrecords);
        if (message.getErrorCode()) {
            return message;
        }
        do {
            auto _record = cursor.second->Fetch();
            // TODO std::find
            for (auto &record : records) {
                if (_record != record) {
                    continue;
                }

                try {
                    cursor.second->Update(columns, values);
                } catch (std::exception &exception) {
                    return Message(ErrorConstants::ERR_STO);
                }
            }

        } while (!cursor.second->NextRecord());
    }
//    cursor.second->Reset();

    return message;
}
