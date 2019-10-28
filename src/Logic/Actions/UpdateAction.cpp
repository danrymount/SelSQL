//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
#include "../../Parser/Headers/UpdateVisitor.h"
// BigResponse UpdateAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    // response = mainEngine->Update(&_request);
//    cursor = mainEngine->GetCursor(_request->tableName);
//    if (cursor.first->name.empty()) {
//        response.error = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
//        return response;
//    }
//
//    response.error = ActionsUtils::checkFieldsExist(cursor.first, _request->dmlData.columns);
//    if (response.error.getErrorCode()) {
//        return response;
//    }
//
//    response.error = actionsUtils.checkConstraint(_request->dmlData.columns, _request->dmlData.values, cursor);
//    if (response.error.getErrorCode()) {
//        requestToResponse(_request);
//        return response;
//    }
//    cursor.second->Reset();
//
//    auto expr = _request->expression;
//
//    do {
//        auto record = cursor.second->Fetch();
//        // std::cout << cursor.second->current_pos << std::endl;
//        if (expr.first.empty()) {
//            cursor.second->Update(_request->dmlData.columns, _request->dmlData.values);
//
//        } else {
//            RecordsData row;
//            row.emplace_back(record);
//            auto data = actionsUtils.checkExpression(expr, row);
//            if (data.empty())
//                continue;
//            cursor.second->Update(_request->dmlData.columns, _request->dmlData.values);
//        }
//
//        //        response.error = actionsUtils.checkConstraint(_request.dmlData.columns, _request.dmlData.values,
//        //        cursor); if (response.error.getErrorCode()) {
//        //            requestToResponse(_request);
//        //            return response;
//        //        }
//        //        cursor.second->Reset();
//
//    } while (!cursor.second->Next());
//
//    cursor.second->Commit();
//
//    return response;
//}
Message UpdateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<UpdateVisitor *>(getTreeVisitor().get());
    auto updateColumns = v->getUpdates();
    auto expr = v->getExpr();

    cursor = getEngine().GetCursor(root->getTableName());
    auto table = cursor.first;
    if (table->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    message = ActionsUtils::checkFieldsExist(table, updateColumns);
    if (message.getErrorCode()) {
        return message;
    }

    std::vector<ActionsUtils::Record> records;
    // = ActionsUtils::getAllRecords(cursor);
    // cursor.second->Reset();
    if (cursor.first->record_amount) {
        do {
            auto record = cursor.second->Fetch();
            if (record.empty()) {
                continue;
            }
            v->setValues(record);
            expr->accept(getTreeVisitor().get());
            if (v->getResult()) {
                records.emplace_back(record);
            }
        } while (!cursor.second->Next());
    }
    cursor.second->Reset();

    do {
        auto _record = cursor.second->Fetch();
        for (auto &record : records) {
            if (_record != record) {
                continue;
            }
            message = actionsUtils.checkConstraint(updateColumns, cursor.first, records, true);
            if (message.getErrorCode()) {
                return message;
            }
            // TODO сменить входные параметры
            std::vector<std::string> columns;
            std::vector<std::string> values;
            for (auto &colValue : updateColumns) {
                columns.emplace_back(colValue.first);
                values.emplace_back(colValue.second);
            }
            cursor.second->Update(columns, values);
        }

    } while (!cursor.second->Next());

    cursor.second->Reset();

    //    do {
    //        auto record = cursor.second->Fetch();
    //
    //        if (record.empty()) {
    //            continue;
    //        }
    //
    //        v->setValues(record);
    //        expr->accept(getTreeVisitor().get());
    //        if (v->getResult()) {
    //            error = actionsUtils.checkConstraint(updateColumns, cursor.first, records, true);
    //            if (error.getErrorCode()) {
    //                return error;
    //            }
    //            // TODO сменить входные параметры
    //            std::vector<std::string> columns;
    //            std::vector<std::string> values;
    //            for (auto &colValue : updateColumns) {
    //                columns.emplace_back(colValue.first);
    //                values.emplace_back(colValue.second);
    //            }
    //            cursor.second->Update(columns, values);
    //        }
    //
    //    } while (!cursor.second->Next());

    cursor.second->Reset();

    return message;
}
