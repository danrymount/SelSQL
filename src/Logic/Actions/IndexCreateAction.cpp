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

    // table->setIndexField(colName); Ставим что такое то поле теперь индексное.
    // v->getEngine()->UpdateTableMeta(table); // обновить таблицу на диске. можно тут можно в конце.

    // дальше, надо сделать селект каждой записи и вызвать метод курсора
    auto data_manager = cursor.second->GetDataManager();
    //    data_manager->CreateIndex(tableName,Тип поля колонки);

    //ДЕЛАешь фетч, получаешь запись. извлекаешь значение в поле индекса из этой записи
    std::string val;  // значение индексного поля, которое надо добавить
    //    data_manager->InsertIndex(tableName,val,cursor.second->GetCurrentPos()); Кьюрент поз, это позиция того, что ты
    //    только что зафетчил
    // когда ты в инсертах, апдейтах, будешь делать надо будет сделать так, что после инсерта или апдейта если таблица у
    // нас с индексами надо также вызывать сразу же только уже вот это
    // data_manager->InsertIndex(tableName,val,cursor.second->GetLastInsertedPos());
    return Message();
}
