//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
#include "../../Parser/Headers/UpdateVisitor.h"

Message UpdateAction::execute(std::shared_ptr<BaseActionNode> root) {
    auto v = static_cast<UpdateVisitor *>(getTreeVisitor().get());
    v->setExpressionVisitor(exprVisitor);
    root->accept(getTreeVisitor().get());
    auto updateColumns = v->getUpdates();
    auto expr = v->getExpr();
    cursor = v->getEngine()->GetCursor(v->getTableName(), root->getId());
    auto table = cursor.first;
    if (table->name.empty()) {
        commitTransaction(root);

        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    message = ActionsUtils::checkFieldsExist(table, updateColumns);
    if (message.getErrorCode()) {
        commitTransaction(root);

        return message;
    }

    std::vector<ActionsUtils::Record> records;
    std::vector<ActionsUtils::Record> allrecords;
    // = ActionsUtils::getAllRecords(cursor);
    // cursor.second->Reset();
    //    if (cursor.first->record_amount) {
    do {
        auto record = cursor.second->Fetch();
        if (record.empty()) {
                continue;
            }
            std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
            for (auto &col : record) {
                _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
            }
            exprVisitor->setFirstValues(_newRecord);
            try {
                expr->accept(exprVisitor);
            } catch (std::exception &exception) {
                std::string exc = exception.what();
                commitTransaction(root);

                return Message(ErrorConstants::ERR_TYPE_MISMATCH);
            }
            if (exprVisitor->getResult()) {
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
            commitTransaction(root);

            return message;
        }
        do {
            auto _record = cursor.second->Fetch();
            auto rec = std::find(records.begin(), records.end(), _record);
            if (rec == records.end()) {
                continue;
            }

            try {
                if (cursor.second->Update(columns, values) == ErrorConstants::ERR_TRANSACT_CONFLICT) {
                    commitTransaction(root);

                    return Message(ErrorConstants::ERR_TRANSACT_CONFLICT);
                };

            } catch (std::exception &exception) {
                commitTransaction(root);

                return Message(ErrorConstants::ERR_STO);
            }

        } while (!cursor.second->NextRecord());

        return message;
}
