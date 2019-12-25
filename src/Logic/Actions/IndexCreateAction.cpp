//
// Created by sapiest on 24.12.2019.
//

#include "Headers/IndexCreateAction.h"
#include "../../Parser/Headers/IndexCreateVisitor.h"

Message IndexCreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<IndexCreateVisitor *>(getTreeVisitor().get());
    auto tableName = v->getTableName();
    auto colName = v->getColName();
    auto cursor = v->getEngine()->GetCursor(tableName, root->getId());
    auto table = cursor.first;

    table->setIndexField(colName);
    v->getEngine()->UpdateTableMeta(table);  // обновить таблицу на диске. можно тут можно в конце.

    // дальше, надо сделать селект каждой записи и вызвать метод курсора
    auto data_manager = cursor.second->GetDataManager();
    auto it = std::find_if(table->getFields().begin(), table->getFields().end(),
                           [colName](std::pair<std::string, Variable> field) { return field.first == colName; });
    if (it == table->getFields().end()) {
        // TODO нет такой колонки
        return Message();
    }

    data_manager->CreateIndex(tableName, it->second.type);

    do {
        auto _record = cursor.second->Fetch();
        if (_record.first.empty()) {
            continue;
        }
        auto indexCol = std::find_if(_record.first.begin(), _record.first.end(),
                                     [colName](const std::pair<std::string, std::string> &val) {
                                         return val.first == colName;
                                     });
        if (indexCol != _record.first.end()) {
            auto val = indexCol->second;
            data_manager->InsertIndex(tableName, val, cursor.second->GetCurrentPos());
        }
    } while (!cursor.second->NextRecord());

    //    data_manager->InsertIndex(tableName,val,cursor.second->GetCurrentPos()); Кьюрент поз, это позиция того, что ты
    //    только что зафетчил
    // когда ты в инсертах, апдейтах, будешь делать надо будет сделать так, что после инсерта или апдейта если таблица у
    // нас с индексами надо также вызывать сразу же только уже вот это
    // data_manager->InsertIndex(tableName,val,cursor.second->GetLastInsertedPos());
    return Message();
}
