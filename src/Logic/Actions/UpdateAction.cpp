//
// Created by sapiest on 05.10.2019.
//

#include "Headers/UpdateAction.h"
#include "../../Parser/Headers/UpdateVisitor.h"
// BigResponse UpdateAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    // response = mainEngine->Update(&_request);
//    cursor = mainEngine->GetCursor(_request->tableName);
//    if (cursor.first->name.empty()) {
//        response.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
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
//    cursor.second->StartPos();
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
//        //        cursor.second->StartPos();
//
//    } while (!cursor.second->Next());
//
//    cursor.second->Commit();
//
//    return response;
//}
Error UpdateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->getChild()->accept(getTreeVisitor().get());
    auto v = static_cast<UpdateVisitor *>(getTreeVisitor().get());
    auto updateColumns = v->getUpdates();
    auto expr = v->getExpr();

    cursor = getEngine().GetCursor(root->getTableName());
    auto table = cursor.first;
    if (table->name.empty()) {
        return Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    error = ActionsUtils::checkFieldsExist(table, updateColumns);
    if (error.getErrorCode()) {
        return error;
    }

    error = actionsUtils.checkConstraint(updateColumns, cursor);
    if (error.getErrorCode()) {
        return error;
    }

    do {
        auto record = cursor.second->Fetch();
        if (record.empty()) {
            continue;
        }
        v->setValues(record);
        expr->accept(getTreeVisitor().get());
        if (v->getResult()) {
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
}
