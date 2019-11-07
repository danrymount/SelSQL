//
// Created by toxaxab on 29.10.2019.
//

#ifndef SELSQL_DROPVISITOR_H
#define SELSQL_DROPVISITOR_H
#include "../Nodes/ActionNodes/DropNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "TreeVisitor.h"
class DropVisitor : public TreeVisitor {
   public:
    Message visitTemplate(DropNode* node) override {
        node->getSource()->accept(this);
        return Message();
    }

    // void visit(DropNode* node) override { node->getSource()->accept(this); }

    void visit(IdentNode* node) override { tableName = node->getBaseValue(); }

    std::string getTableName() { return tableName; }

   private:
    std::string tableName;
};
#endif  // SELSQL_DROPVISITOR_H
