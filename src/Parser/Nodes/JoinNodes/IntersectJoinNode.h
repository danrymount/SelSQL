//
// Created by sapiest on 11.11.2019.
//

#ifndef SELSQL_INTERSECTJOINNODE_H
#define SELSQL_INTERSECTJOINNODE_H
#include "UnionIntersectNode.h"
class IntersectJoinNode : public UnionIntersectNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_INTERSECTJOINNODE_H
