//
// Created by toxaxab on 29.10.2019.
//

#ifndef SELSQL_JOINNODE_H
#define SELSQL_JOINNODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
#include "BaseJoinNode.h"
class JoinNode : public BaseJoinNode {
   public:

    void accept(TreeVisitor* v) override {
        // TODO
    }
};
#endif  // SELSQL_JOINNODE_H
