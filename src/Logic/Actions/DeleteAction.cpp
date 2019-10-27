//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DeleteAction.h"
#include "../../Parser/Headers/DeleteVisitor.h"

// BigResponse DeleteAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
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
//    int delete_count = 0;
//
//    auto expr = _request->expression;
//
//    if (cursor.first->record_amount == 0) {
//        return response;
//    }
//    do {
//        auto record = cursor.second->Fetch();
//        if (expr.first.empty()) {
//            delete_count++;
//            cursor.second->Delete();
//            continue;
//        } else {
//            RecordsData row;
//            row.emplace_back(record);
//            auto data = actionsUtils.checkExpression(expr, row);
//            if (data.empty())
//                continue;
//            delete_count++;
//            cursor.second->Delete();
//        }
//
//    } while (!cursor.second->Next());
//
//    cursor.second->table->record_amount -= delete_count;
//    cursor.second->Commit();
//    return response;
//}

Message DeleteAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<DeleteVisitor*>(getTreeVisitor().get());
    auto expr = v->getExpr();

    cursor = getEngine().GetCursor(root->getTableName());
    if (cursor.first->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    int delete_count = 0;

    if (cursor.first->record_amount == 0) {
        cursor.second.reset();
        return message;
    }

    do {
        //TODO Получает пустые записи
        auto record = cursor.second->Fetch();
        if (record.empty()) {
            continue;
        }

        v->setValues(record);
        expr->accept(getTreeVisitor().get());
        if (v->getResult()) {
            //            delete_count++;
            cursor.second->Delete();
        }

    } while (!cursor.second->Next());

    //    cursor.second->table->record_amount -= delete_count;
    //    cursor.second->Commit();
    //    cursor.second.reset();
    return message;
}
