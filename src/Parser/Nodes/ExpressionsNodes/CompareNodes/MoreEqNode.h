//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_MOREEQNODE_H
#define SELSQL_MOREEQNODE_H
#include "CmpNode.h"
class MoreEqNode : public CmpNode {
   public:
    MoreEqNode() = default;
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_MOREEQNODE_H
