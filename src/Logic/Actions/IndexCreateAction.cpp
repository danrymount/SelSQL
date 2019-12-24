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
}
