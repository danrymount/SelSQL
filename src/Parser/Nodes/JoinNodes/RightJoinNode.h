//
// Created by sapiest on 07.11.2019.
//

#ifndef SELSQL_RIGHTJOINNODE_H
#define SELSQL_RIGHTJOINNODE_H
#include "BaseJoinNode.h"
class RightJoinNode : public BaseJoinNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_RIGHTJOINNODE_H
