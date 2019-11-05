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
    void visit(DeleteNode* node) override {
        node->getSource()->accept(this);
        tableName = std::move(curValue);
        expr = static_cast<BaseExprNode*>(node->getChild());
    }

    void visit(IdentNode* node) override { curValue = node->getBaseValue(); }

    //    void visit(IndentExprNode* node) override {
    //        int flag = 0;
    //        for (auto& val : values) {
    //            if (node->getBaseValue() == val.first) {
    //                curValue = val.second;
    //                flag = 1;
    //                break;
    //            }
    //        }
    //        if (!flag) {
    //            result = false;
    //            error = Message(ErrorConstants::ERR_NO_SUCH_FIELD);
    //        }
    //    }

    std::string getTableName() { return tableName; }

    BaseExprNode* getExpr() { return expr; }


   private:
    std::string curValue;
    std::string tableName;
    Message error;
    BaseExprNode* expr;
};
#endif  // SELSQL_DELETEVISITOR_H
