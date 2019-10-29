//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_UPDATESANDEXPRNODE_H
#define SELSQL_UPDATESANDEXPRNODE_H
#include "BaseNode.h"
#include "ExpressionsNodes/UpdateExprNode.h"
class UpdatesAndExprNode : public BaseNode {
   public:
    UpdatesAndExprNode(UpdateExprNode* _updates, BaseExprNode* _expr) : updates(_updates), expr(_expr) {}
    ~UpdatesAndExprNode() override {
        if (updates != nullptr) {
            delete updates;
        }
        if (expr != nullptr) {
            delete expr;
        }
    }

    void accept(TreeVisitor* v) override { v->visit(this); }

    UpdateExprNode* getUpdates() { return updates; }
    BaseExprNode* getExpr() { return expr; }

   private:
    UpdateExprNode* updates;
    BaseExprNode* expr;
};
#endif  // SELSQL_UPDATESANDEXPRNODE_H
