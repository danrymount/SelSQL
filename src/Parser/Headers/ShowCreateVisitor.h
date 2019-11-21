//
// Created by toxaxab on 29.10.2019.
//

#ifndef SELSQL_SHOWCREATEVISITOR_H
#define SELSQL_SHOWCREATEVISITOR_H
#include "../Nodes/ActionNodes/ShowCreateNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "TreeVisitor.h"
class ShowCreateVisitor : public TreeVisitor {
   public:
    explicit ShowCreateVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)){};

    void visit(ShowCreateNode* node) override { node->getSource()->accept(this); }

    void visit(IdentNode* node) override { tableName = node->getBaseValue(); }

    std::string getTableName() { return tableName; }

   private:
    std::string tableName;
};
#endif  // SELSQL_SHOWCREATEVISITOR_H
