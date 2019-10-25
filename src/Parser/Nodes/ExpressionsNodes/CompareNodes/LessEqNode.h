//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_LESSEQNODE_H
#define SELSQL_LESSEQNODE_H
#include "CmpNode.h"
class LessEqNode : public CmpNode {
   public:
    LessEqNode() = default;
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_LESSEQNODE_H
