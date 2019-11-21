//
// Created by sapiest on 24.10.2019.
//

#ifndef SELSQL_DELETEVISITOR_H
#define SELSQL_DELETEVISITOR_H
#include "../Nodes/ActionNodes/DeleteNode.h"
#include "../Nodes/ExpressionsNodes/IndentExprNode.h"
#include "TreeVisitor.h"
class DeleteVisitor : public TreeVisitor {
   public:
    explicit DeleteVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)){};

    void visit(DeleteNode* node) override {
        node->getSource()->accept(this);
        tableName = std::move(curValue);
        expr = static_cast<BaseExprNode*>(node->getChild());
    }

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    std::string getTableName() { return tableName; }

    BaseExprNode* getExpr() { return expr; }

   private:
    std::string curValue;
    std::string tableName;
    Message error;
    BaseExprNode* expr = nullptr;
};
#endif  // SELSQL_DELETEVISITOR_H
