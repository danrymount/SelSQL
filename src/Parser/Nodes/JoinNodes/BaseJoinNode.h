//
// Created by toxaxab on 28.10.2019.
//

#ifndef SELSQL_BASEJOINNODE_H
#define SELSQL_BASEJOINNODE_H
#include "../BaseNode.h"
class BaseJoinNode : public BaseNode {
   public:
    BaseJoinNode() = default;

    void accept(TreeVisitor* v) override {
        // TODO
    }

    void addChilds(BaseJoinNode* _firstSource, BaseJoinNode* _secondSource, BaseExprNode* _expr) {
        firstSourceJoinNode = _firstSource;
        secondSourceJoinNode = _secondSource;
        expr = _expr;
    }

    BaseExprNode* getExpr() { return expr; }

    BaseJoinNode* getFirstSource() { return firstSourceJoinNode; }

    BaseJoinNode* getSecondSource() { return secondSourceJoinNode; }

   private:
    BaseJoinNode* firstSourceJoinNode = nullptr;
    BaseJoinNode* secondSourceJoinNode = nullptr;
    BaseExprNode* expr = nullptr;
};

#endif  // SELSQL_BASEJOINNODE_H
