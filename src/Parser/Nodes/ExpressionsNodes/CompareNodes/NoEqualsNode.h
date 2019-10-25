//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_NOEQUALSNODE_H
#define SELSQL_NOEQUALSNODE_H
#include "CmpNode.h"
class NoEqualsNode : public CmpNode {
   public:
    NoEqualsNode() = default;
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_NOEQUALSNODE_H
