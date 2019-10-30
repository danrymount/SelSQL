//
// Created by toxaxab on 30.10.2019.
//

#ifndef SELSQL_TABLENODE_H
#define SELSQL_TABLENODE_H
#include "ExpressionsNodes/IndentNode.h"
class TableNode : public BaseNode {
   public:
    TableNode(IdentNode* _node) : tableName(_node) {}
    ~TableNode() override { delete tableName; }

    void accept(TreeVisitor* v) override { v->visit(this); }

    IdentNode* getChild() { return tableName; }

   private:
    IdentNode* tableName = nullptr;
};

#endif  // SELSQL_TABLENODE_H
