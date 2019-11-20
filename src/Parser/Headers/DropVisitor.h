//
// Created by toxaxab on 29.10.2019.
//

#ifndef SELSQL_DROPVISITOR_H
#define SELSQL_DROPVISITOR_H
#include <utility>

#include "../Nodes/ActionNodes/DropNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "TreeVisitor.h"
class DropVisitor : public TreeVisitor {
   public:
    explicit DropVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)) {}

    void visit(DropNode* node) override { node->getSource()->accept(this); }

    void visit(IdentNode* node) override { tableName = node->getBaseValue(); }

    std::string getTableName() { return tableName; }

   private:
    std::string tableName;
};
#endif  // SELSQL_DROPVISITOR_H
