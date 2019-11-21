//
// Created by sapiest on 07.11.2019.
//

#ifndef SELSQL_LEFTJOINNODE_H
#define SELSQL_LEFTJOINNODE_H
#include "BaseJoinNode.h"
class LeftJoinNode : public BaseJoinNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_LEFTJOINNODE_H
