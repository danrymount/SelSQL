//
// Created by sapiest on 07.11.2019.
//

#ifndef SELSQL_FULLJOINNODE_H
#define SELSQL_FULLJOINNODE_H
#include "BaseJoinNode.h"
class FullJoinNode : public BaseJoinNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_FULLJOINNODE_H
