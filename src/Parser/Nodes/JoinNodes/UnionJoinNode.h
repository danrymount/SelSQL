//
// Created by sapiest on 11.11.2019.
//

#ifndef SELSQL_UNIONJOINNODE_H
#define SELSQL_UNIONJOINNODE_H
#include "UnionIntersectNode.h"
class UnionJoinNode : public UnionIntersectNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_UNIONJOINNODE_H
