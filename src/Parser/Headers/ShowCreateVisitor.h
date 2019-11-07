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
    Message visitTemplate(ShowCreateNode* node) override {
        node->getSource()->accept(this);
        return Message();
    }

    // void visit(ShowCreateNode* node) override { node->getSource()->accept(this); }

    void visit(IdentNode* node) override { tableName = node->getBaseValue(); }

    std::string getTableName() { return tableName; }

   private:
    std::string tableName;
};
#endif  // SELSQL_SHOWCREATEVISITOR_H
