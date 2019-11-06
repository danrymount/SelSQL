//
// Created by sapiest on 05.10.2019.
//

#include "Headers/DeleteAction.h"
#include "../../Parser/Headers/DeleteVisitor.h"

Message DeleteAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<DeleteVisitor *>(getTreeVisitor().get());
    auto expr = v->getExpr();

    cursor = getEngine().GetCursor(v->getTableName());
    if (cursor.first->name.empty()) {
        return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    int delete_count = 0;

    if (cursor.first->record_amount == 0) {
        cursor.second.reset();
        return Message();
    }

    do {
        // TODO Получает пустые записи
        auto record = cursor.second->Fetch();
        if (record.empty()) {
            continue;
        }

        v->setValues(record);
        try {
            expr->accept(v);
        } catch (std::exception &exception) {
            std::string exc = exception.what();
            return Message(ErrorConstants::ERR_TYPE_MISMATCH);
        }
        if (v->getResult()) {
            //            delete_count++;
            cursor.second->Delete();
        }

    } while (!cursor.second->NextRecord());

    //    cursor.second->table->record_amount -= delete_count;
    //    cursor.second->Commit();
    //    cursor.second.reset();
    return Message();
}
